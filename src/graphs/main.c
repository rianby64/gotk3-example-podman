
#include "node.h"

#define GDK_EVENT(obj)	 		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GDK_TYPE_EVENT, GdkEvent))

GtkWidget *canvas = NULL;
gint       last_id = 0;

static void
cleanup (void)
{
  gtk_main_quit ();
}

static void
item1_activate_cb (GtkMenuItem *menuitem,
                   gpointer     user_data)
{
  GString *label = g_string_new (NULL);
  g_string_printf (label, "%d", ++last_id);

  GooCanvasItem *gnodes = GOO_CANVAS_ITEM (user_data);
  create_node (canvas, gnodes, drag_x, drag_y, label->str);
}

static void
item2_activate_cb (GtkMenuItem *menuitem,
                   gpointer     user_data)
{
  g_print("item2 activate\n");
}

static gboolean
context_menu_cb (GtkWidget      *widget,
                 GdkEventButton *event,
                 gpointer        user_data)
{
  drag_x = event->x;
  drag_y = event->y;

  if (called_from_vertex) {
    called_from_vertex = FALSE;

    GtkWidget *menu;
    menu = gtk_menu_new ();

    GtkWidget *item2;
    item2 = gtk_menu_item_new_with_label ("Connect");
    g_signal_connect (GTK_MENU_ITEM (item2),
                      "activate",
                      G_CALLBACK(item2_activate_cb),
                      GOO_CANVAS_ITEM (user_data));

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), item2);
    gtk_widget_show (item2);

    gtk_menu_popup_at_pointer (GTK_MENU (menu), NULL);

    return GDK_EVENT_PROPAGATE;
  };

  if (event->button == GDK_BUTTON_SECONDARY) {
    GtkWidget *menu;
    menu = gtk_menu_new ();

    GtkWidget *item1;
    item1 = gtk_menu_item_new_with_label ("Add");
    g_signal_connect (GTK_MENU_ITEM (item1),
                      "activate",
                      G_CALLBACK(item1_activate_cb),
                      GOO_CANVAS_ITEM (user_data));

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), item1);
    gtk_widget_show (item1);

    gtk_menu_popup_at_pointer (GTK_MENU (menu), NULL);

    return GDK_EVENT_STOP;
  }
  return GDK_EVENT_PROPAGATE;
}

static GtkWidget*
create_window_canvas (void)
{

  GtkWidget *window, *scrolled_win, *local_canvas;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window,
                    "destroy",
                    G_CALLBACK (cleanup),
                    NULL);

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win), GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_container_add (GTK_CONTAINER (window), scrolled_win);

  local_canvas = goo_canvas_new ();
  gtk_widget_set_size_request (local_canvas, 600, 450);
  goo_canvas_set_bounds (GOO_CANVAS (local_canvas), 0, 0, 1000, 1000);
  gtk_widget_show (local_canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), local_canvas);

  return local_canvas;
}

/*
static void
connect_two_nodes (GooCanvasItem *gedges,
                   GooCanvasItem *node1,
                   GooCanvasItem *node2)
{
  gdouble x1, y1, x2, y2;
  g_object_get (G_OBJECT(node1),
                "x", &x1,
                "y", &y1,
                NULL);
  g_object_get (G_OBJECT(node2),
                "x", &x2,
                "y", &y2,
                NULL);

  GString *path = g_string_new (NULL);
  g_string_printf (path, "M %d %d L %d %d",
                   (int)x1,
                   (int)y1,
                   (int)x2,
                   (int)y2);

  GooCanvasItem *p = goo_canvas_path_new (gedges,
                                          path->str,
                                          "stroke-color", "green",
                                          NULL);
}
*/

int
main (int argc, char *argv[])
{
  GString *label = g_string_new (NULL);
  GooCanvasItem *root;

  gtk_init (&argc, &argv);

  canvas = create_window_canvas ();
  root = goo_canvas_get_root_item (GOO_CANVAS (canvas));

  goo_canvas_group_new (root,
                                 "x", 0.0,
                                 "y", 0.0,
                                 NULL);

  GooCanvasItem *gnodes;
	gnodes = goo_canvas_group_new (root,
                                 "x", 0.0,
                                 "y", 0.0,
                                 NULL);

  g_signal_connect (GOO_CANVAS (canvas),
                    "button-release-event",
                    G_CALLBACK (context_menu_cb),
                    GOO_CANVAS_ITEM (gnodes));

  g_string_printf (label, "%d", ++last_id);
  create_node (canvas, gnodes, 100.0, 100.0, label->str);

  g_string_printf (label, "%d", ++last_id);
  create_node (canvas, gnodes, 150.0, 150.0, label->str);

  g_string_printf (label, "%d", ++last_id);
  create_node (canvas, gnodes, 250.0, 150.0, label->str);

  //connect_two_nodes (gedges, node1, node2);

  gtk_main ();
  return 0;
}
