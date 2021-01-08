
#include "node.h"

#define GDK_EVENT(obj)	 		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GDK_TYPE_EVENT, GdkEvent))

GtkWidget *canvas = NULL;
gint       last_id = 0;

static void
cleanup (void)
{
  g_array_free (graph, FALSE);
  gtk_main_quit ();
}

static void
menu_add_btn_activate_cb (GtkMenuItem *menuitem,
                   gpointer     user_data)
{
  GString *label;
  label = g_string_new (NULL);
  g_string_printf (label, "%d", ++last_id);

  GooCanvasItem *gnodes;
  gnodes = GOO_CANVAS_ITEM (user_data);
  create_node (canvas, gnodes, drag_x, drag_y, label->str);
}

static gboolean
last_edge_on_button_press_event_cb (GooCanvasItem  *item,
                                    GooCanvasItem  *target_item,
                                    GdkEventButton *event,
                                    gpointer        user_data)
{
  goo_canvas_item_remove (item);
  started_connect_vertex = FALSE;
  last_edge = NULL;

  return GDK_EVENT_PROPAGATE;
}

static void
menu_connect_btn_activate_cb (GtkMenuItem *menuitem,
                   gpointer     user_data)
{
  if (last_edge != NULL) {
    return;
  }

  gdouble x, y;

  start_node = last_node;
  started_connect_vertex = TRUE;

  g_object_get (G_OBJECT (start_node),
                "x", &x,
                "y", &y,
                NULL);

  GString *path;
  path = g_string_new (NULL);
  g_string_printf (path, "M %d %d L %d %d",
                   (int)x,
                   (int)y,
                   (int)x,
                   (int)y);

  GooCanvasItem *gedges;
  gedges = GOO_CANVAS_ITEM (user_data);

  last_edge = goo_canvas_path_new (gedges,
                                   path->str,
                                   "stroke-color", "green",
                                   NULL);

  g_signal_connect (last_edge,
                    "button-press-event",
                    G_CALLBACK (last_edge_on_button_press_event_cb),
                    NULL);
}

static gboolean
context_connect_menu_cb (GtkWidget      *widget,
                         GdkEventButton *event,
                         gpointer        user_data)
{
  if (called_from_vertex) {
    called_from_vertex = FALSE;

    GtkWidget *menu;
    menu = gtk_menu_new ();

    GtkWidget *item2;
    item2 = gtk_menu_item_new_with_label ("Connect");
    g_signal_connect (GTK_MENU_ITEM (item2),
                      "activate",
                      G_CALLBACK(menu_connect_btn_activate_cb),
                      GOO_CANVAS_ITEM (user_data));

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), item2);
    gtk_widget_show (item2);

    gtk_menu_popup_at_pointer (GTK_MENU (menu), NULL);

    return GDK_EVENT_STOP;
  };

  return GDK_EVENT_PROPAGATE;
}

static gboolean
context_add_menu_cb (GtkWidget      *widget,
                     GdkEventButton *event,
                     gpointer        user_data)
{
  if (event->button == GDK_BUTTON_SECONDARY) {
    drag_x = event->x;
    drag_y = event->y;

    GtkWidget *menu;
    menu = gtk_menu_new ();

    GtkWidget *item1;
    item1 = gtk_menu_item_new_with_label ("Add");
    g_signal_connect (GTK_MENU_ITEM (item1),
                      "activate",
                      G_CALLBACK(menu_add_btn_activate_cb),
                      GOO_CANVAS_ITEM (user_data));

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), item1);
    gtk_widget_show (item1);

    gtk_menu_popup_at_pointer (GTK_MENU (menu), NULL);

    return GDK_EVENT_STOP;
  }

  return GDK_EVENT_PROPAGATE;
}

static gboolean
on_motion_notify_event_cb (GtkWidget      *widget,
                           GdkEventMotion *event,
                           gpointer        user_data)
{
  if (started_connect_vertex && !dragged_vertex) {
    gdouble x, y;
    g_object_get (G_OBJECT (start_node),
                  "x", &x,
                  "y", &y,
                  NULL);

    GString *data;
    data = g_string_new (NULL);
    g_string_printf (data,
                     "M %d %d L %d %d",
                     (int)x,
                     (int)y,
                     (int)event->x,
                     (int)event->y);

    g_object_set (G_OBJECT (last_edge),
                    "data",
                    data->str,
                    NULL);

    dragged_vertex = FALSE;

    return GDK_EVENT_STOP;
  }

  dragged_vertex = FALSE;

  return GDK_EVENT_PROPAGATE;
}

static GtkWidget*
create_window_canvas (void)
{
  GtkWidget *window, *scrolled_win, *local_canvas;
  GooCanvasItem *root, *gnodes, *gedges;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);
  gtk_widget_show (window);
  g_signal_connect (GTK_WINDOW (window),
                    "destroy",
                    G_CALLBACK (cleanup),
                    NULL);

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win), GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_container_add (GTK_CONTAINER (window), scrolled_win);

  local_canvas = goo_canvas_new ();
  gtk_widget_set_size_request (local_canvas, 300, 300);
  goo_canvas_set_bounds (GOO_CANVAS (local_canvas), 0, 0, 300, 300);
  gtk_widget_show (local_canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), local_canvas);

  root = goo_canvas_get_root_item (GOO_CANVAS (local_canvas));
  gedges = goo_canvas_group_new (root,
                                 "x", 0.0,
                                 "y", 0.0,
                                 NULL);
	gnodes = goo_canvas_group_new (root,
                                 "x", 0.0,
                                 "y", 0.0,
                                 NULL);

  g_signal_connect (GOO_CANVAS (local_canvas),
                    "button-release-event",
                    G_CALLBACK (context_connect_menu_cb),
                    GOO_CANVAS_ITEM (gedges));

  g_signal_connect (GOO_CANVAS (local_canvas),
                    "button-release-event",
                    G_CALLBACK (context_add_menu_cb),
                    GOO_CANVAS_ITEM (gnodes));

  g_signal_connect (window,
                    "motion-notify-event",
                    G_CALLBACK (on_motion_notify_event_cb),
                    GOO_CANVAS_ITEM (gedges));

  return local_canvas;
}

int
main (int argc, char *argv[])
{
  gtk_init (&argc, &argv);

  canvas = create_window_canvas ();
  graph = g_array_new (FALSE, FALSE, sizeof (Graph));

  gtk_main ();
  return 0;
}
