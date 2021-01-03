
#include <stdlib.h>
#include <goocanvas.h>

GooCanvasItem   *drag_item = NULL;
gdouble          drag_x = 0.0;
gdouble          drag_y = 0.0;
gdouble          item_x = 0.0;
gdouble          item_y = 0.0;

static gboolean
on_button_press_event_cb (GooCanvasItem *item,
                          GooCanvasItem *target_item,
                          GdkEventButton *event,
                          gpointer user_data)
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

      goo_canvas_pointer_grab (GOO_CANVAS (user_data), item, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_RELEASE_MASK, NULL, event->time);
      return TRUE;
  }

  return FALSE;
}

static gboolean
on_button_release_event_cb (GooCanvasItem  *item,
                            GooCanvasItem  *target_item,
                            GdkEventButton *event,
                            gpointer user_data)
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
                           gpointer user_data)
{
  if (drag_item == item && drag_item != NULL)
  {
    gdouble rel_x = event->x - drag_x;
    gdouble rel_y = event->y - drag_y;

    g_object_set (G_OBJECT (item), "x", item_x + rel_x, "y", item_y + rel_y, NULL);
    return TRUE;
  }

  return FALSE;
}

static void
setup_dnd_handlers (GooCanvas *canvas,
                    GooCanvasItem *item)
{
  g_signal_connect (G_OBJECT (item), "button-press-event", G_CALLBACK (on_button_press_event_cb), canvas);
  g_signal_connect (G_OBJECT (item), "button-release-event", G_CALLBACK (on_button_release_event_cb), canvas);
  g_signal_connect (G_OBJECT (item), "motion-notify-event", G_CALLBACK (on_motion_notify_event_cb), canvas);
}


int
main (int argc, char *argv[])
{

  GtkWidget *window, *scrolled_win, *canvas;
  GooCanvasItem *root, *rect_item, *text_item, *parent1;
  GooCanvasItem* child;

  gtk_init (&argc, &argv);

  /* Create the window and widgets. */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win),
				       GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_container_add (GTK_CONTAINER (window), scrolled_win);

  canvas = goo_canvas_new ();
  gtk_widget_set_size_request (canvas, 600, 450);
  goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 1000, 1000);
  gtk_widget_show (canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

  root = goo_canvas_get_root_item (GOO_CANVAS (canvas));


	parent1 = goo_canvas_group_new (root,
    "x", 100.0,
    "y", 100.0,
    NULL);

  child = goo_canvas_ellipse_new (parent1, 0, 0, 60, 60,
    "fill-color", "red",
    "line-width", 1.0,
    NULL);

  text_item = goo_canvas_text_new (parent1, "A", 0, 0, -1,
				   GOO_CANVAS_ANCHOR_CENTER,
				   "font", "Sans 24",
				   NULL);

  setup_dnd_handlers (GOO_CANVAS (canvas), parent1);

  gtk_main ();
  return 0;
}
