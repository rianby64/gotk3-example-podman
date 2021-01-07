
#ifndef GRAPH_NODE_C
#define GRAPH_NODE_C

#include "node.h"

GooCanvasItem *drag_item = NULL;
gdouble        item_x = 0.0;
gdouble        item_y = 0.0;
const gint     node_RADIUS = 30;

gdouble  drag_x = 0.0;
gdouble  drag_y = 0.0;
gboolean called_from_vertex = FALSE;
gboolean started_connect_vertex = FALSE;
GooCanvasItem *last_node = NULL;

static gboolean
on_button_press_event_cb (GooCanvasItem  *item,
                          GooCanvasItem  *target_item,
                          GdkEventButton *event,
                          gpointer        user_data)
{
  last_node = GOO_CANVAS_ITEM (item);

  if (event->button == GDK_BUTTON_SECONDARY)
  {
    called_from_vertex = TRUE;
    return GDK_EVENT_STOP;
  }

  if (event->button == GDK_BUTTON_PRIMARY)
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
      return GDK_EVENT_STOP;
  }
  return GDK_EVENT_PROPAGATE;
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
    return GDK_EVENT_STOP;
  }

  return GDK_EVENT_PROPAGATE;
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

    g_object_set (G_OBJECT (item),
                  "x", x,
                  "y", y,
                  NULL);

    return GDK_EVENT_STOP;
  }

  return GDK_EVENT_PROPAGATE;
}

static void
setup_dnd_handlers (GooCanvas     *local_canvas,
                    GooCanvasItem *item)
{
  g_signal_connect (GOO_CANVAS_ITEM (item),
                    "button-press-event",
                    G_CALLBACK (on_button_press_event_cb),
                    GOO_CANVAS (local_canvas));
  g_signal_connect (GOO_CANVAS_ITEM (item),
                    "button-release-event",
                    G_CALLBACK (on_button_release_event_cb),
                    GOO_CANVAS (local_canvas));
  g_signal_connect (GOO_CANVAS_ITEM (item),
                    "motion-notify-event",
                    G_CALLBACK (on_motion_notify_event_cb),
                    GOO_CANVAS (local_canvas));
}

GooCanvasItem*
create_node (GtkWidget *local_canvas,
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

  setup_dnd_handlers (GOO_CANVAS (local_canvas), g);
  return g;
}

#endif
