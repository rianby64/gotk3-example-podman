
#include <stdlib.h>
#include <goocanvas.h>

GooCanvasItem   *drag_item = NULL;
gdouble          drag_x = 0.0;
gdouble          drag_y = 0.0;
gdouble          item_x = 0.0;
gdouble          item_y = 0.0;


struct IDObject {
  GooCanvasItem *node;
  GooCanvasItem *edge;
};

GArray *garray = NULL;

static gboolean
on_button_press_event_cb (GooCanvasItem  *item,
                          GooCanvasItem  *target_item,
                          GdkEventButton *event,
                          gpointer        user_data)
{
  if (event->button == 1)
  {
      drag_item = item;
      drag_x = event->x;
      drag_y = event->y;

      g_object_get (G_OBJECT (item),
                    "x", &item_x,
                    "y", &item_y,
                    NULL);

      goo_canvas_pointer_grab (GOO_CANVAS (user_data),
                               item,
                               GDK_POINTER_MOTION_MASK |
                                 GDK_POINTER_MOTION_HINT_MASK |
                                 GDK_BUTTON_RELEASE_MASK,
                               NULL,
                               event->time);
      return TRUE;
  }
  return FALSE;
}

static gboolean
on_button_release_event_cb (GooCanvasItem  *item,
                            GooCanvasItem  *target_item,
                            GdkEventButton *event,
                            gpointer        user_data)
{
  if (drag_item == item && drag_item != NULL)
  {
    goo_canvas_pointer_ungrab (GOO_CANVAS (user_data), drag_item, event->time);
    drag_item = NULL;
    return TRUE;
  }

  return FALSE;
}

static gboolean
on_motion_notify_event_cb (GooCanvasItem  *item,
                           GooCanvasItem  *target_item,
                           GdkEventMotion *event,
                           gpointer        user_data)
{
  if (drag_item == item && drag_item != NULL)
  {
    gdouble rel_x = event->x - drag_x;
    gdouble rel_y = event->y - drag_y;

    gdouble x = item_x + rel_x;
    gdouble y = item_y + rel_y;

    struct IDObject idobject1 = g_array_index (garray, struct IDObject, 0);
    struct IDObject idobject2 = g_array_index (garray, struct IDObject, 1);


    g_object_set (G_OBJECT (item),
                  "x", x,
                  "y", y,
                  NULL);

    if (drag_item == idobject1.node) {
      gdouble px, py;
      g_object_get (G_OBJECT (idobject2.node),
                    "x", &px,
                    "y", &py,
                    NULL);

      GString *string = g_string_new(NULL);
      g_string_printf (string,
                       "M %d %d L %d %d",
                       (int)x,
                       (int)y,
                       (int)px,
                       (int)py);

      g_object_set (G_OBJECT (idobject1.edge),
                    "data",
                    string->str,
                    NULL);
    }

    if (drag_item == idobject2.node) {
      gdouble px, py;
      g_object_get(G_OBJECT (idobject1.node),
                   "x", &px,
                   "y", &py,
                   NULL);

      GString *string = g_string_new(NULL);
      g_string_printf (string,
                       "M %d %d L %d %d",
                       (int)px,
                       (int)py,
                       (int)x,
                       (int)y);

      g_object_set (G_OBJECT(idobject2.edge),
                    "data",
                    string->str,
                    NULL);
    }
    return TRUE;
  }

  return FALSE;
}

static void
setup_dnd_handlers (GooCanvas     *canvas,
                    GooCanvasItem *item)
{
  g_signal_connect (G_OBJECT (item),
                    "button-press-event",
                    G_CALLBACK (on_button_press_event_cb),
                    canvas);
  g_signal_connect (G_OBJECT (item),
                    "button-release-event",
                    G_CALLBACK (on_button_release_event_cb),
                    canvas);
  g_signal_connect (G_OBJECT (item),
                    "motion-notify-event",
                    G_CALLBACK (on_motion_notify_event_cb),
                    canvas);
}

static void
cleanup (void)
{
  g_array_free (garray, TRUE);
  gtk_main_quit ();
}

static GtkWidget*
create_window_canvas (void)
{

  GtkWidget *window, *scrolled_win, *canvas;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window, "destroy", G_CALLBACK (cleanup), NULL);

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win), GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_container_add (GTK_CONTAINER (window), scrolled_win);

  canvas = goo_canvas_new ();
  gtk_widget_set_size_request (canvas, 600, 450);
  goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 1000, 1000);
  gtk_widget_show (canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

  return canvas;
}

int node_RADIUS = 30;

static GooCanvasItem*
create_node (GtkWidget     *canvas,
             GooCanvasItem *parent,
             double         x,
             double         y,
             gchar         *txt)
{

  GooCanvasItem *g;
	g = goo_canvas_group_new (parent,
                            "x", x,
                            "y", y,
                            NULL);

  goo_canvas_ellipse_new (g, 0, 0, node_RADIUS, node_RADIUS,
                          "fill-color", "mediumseagreen",
                          NULL);

  goo_canvas_text_new (g, txt, 0, 0, -1,
				               GOO_CANVAS_ANCHOR_CENTER,
				               "font", "Sans 24",
				               NULL);

  setup_dnd_handlers (GOO_CANVAS (canvas), g);
  return g;
}

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

  struct IDObject id1 = {node1, p};
  struct IDObject id2 = {node2, p};

  g_array_append_val (garray, id1);
  g_array_append_val (garray, id2);
}

int
main (int argc, char *argv[])
{
  GtkWidget *canvas;
  GooCanvasItem *root, *node1, *node2;
  garray = g_array_new (FALSE, FALSE, sizeof (struct IDObject));

  gtk_init (&argc, &argv);

  canvas = create_window_canvas();
  root = goo_canvas_get_root_item (GOO_CANVAS (canvas));

  GooCanvasItem *gedges;
	gedges = goo_canvas_group_new (root,
                                 "x", 0.0,
                                 "y", 0.0,
                                 NULL);

  GooCanvasItem *gnodes;
	gnodes = goo_canvas_group_new (root,
                                 "x", 0.0,
                                 "y", 0.0,
                                 NULL);

  node1 = create_node(canvas, gnodes, 100.0, 100.0, "A");
  node2 = create_node(canvas, gnodes, 150.0, 150.0, "B");
  create_node(canvas, gnodes, 250.0, 150.0, "C");

  connect_two_nodes (gedges, node1, node2);

  gtk_main ();
  return 0;
}
