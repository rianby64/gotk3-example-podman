#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <stdlib.h>
#include <goocanvas.h>

extern gdouble        drag_x;
extern gdouble        drag_y;
extern gboolean       called_from_vertex;
extern gboolean       started_connect_vertex;
extern gboolean       dragged_vertex;
extern GooCanvasItem *last_node;
extern GooCanvasItem *start_node;
extern GooCanvasItem *end_node;
extern GooCanvasItem *last_edge;

extern GooCanvasItem*
create_node (GtkWidget *local_canvas,
             GooCanvasItem *parent,
             gdouble        x,
             gdouble        y,
             gchar         *txt);

#endif
