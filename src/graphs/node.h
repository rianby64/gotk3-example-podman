#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <stdlib.h>
#include <goocanvas.h>

extern gdouble  drag_x;
extern gdouble  drag_y;
extern gboolean called_from_vertex;

extern GooCanvasItem*
create_node (GtkWidget *local_canvas,
             GooCanvasItem *parent,
             double         x,
             double         y,
             gchar         *txt);

#endif
