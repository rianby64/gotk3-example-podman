#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <stdlib.h>
#include <goocanvas.h>

static gdouble  drag_x = 0.0;
static gdouble  drag_y = 0.0;
static gboolean called_from_vertex = FALSE;

GooCanvasItem*
create_node (GtkWidget *local_canvas,
             GooCanvasItem *parent,
             double         x,
             double         y,
             gchar         *txt);

#endif
