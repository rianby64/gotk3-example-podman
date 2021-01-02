
package main

import (
       "fmt"
       "math"

       "github.com/gotk3/gotk3/cairo"
       "github.com/gotk3/gotk3/gdk"
       "github.com/gotk3/gotk3/gtk"
)

func main() {
       gtk.Init(nil)

       // gui boilerplate
       win, _ := gtk.WindowNew(gtk.WINDOW_TOPLEVEL)
       drawingArea, _ := gtk.DrawingAreaNew()
       win.Add(drawingArea)
       win.SetTitle("Arrow keys")
       win.Connect("destroy", gtk.MainQuit)
       win.ShowAll()

       drawingArea.Connect("draw", func(da *gtk.DrawingArea, cr *cairo.Context) {
               cr.SetSourceRGB(0, 50, 0)
               cr.Arc(50, 50, 10, 0, 2*math.Pi)
               cr.ClosePath()
               cr.Fill()

               cr.SelectFontFace("Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_NORMAL)
               cr.SetFontSize(10.0)

               msg := "A"
               extents := cr.TextExtents(msg)
               cr.MoveTo(50-(extents.Height/2), 50+(extents.Width/2))

               cr.SetSourceRGB(0, 0, 50)
               cr.ShowText(msg)
       })

       target, _ := gtk.TargetEntryNew("STRING", gtk.TARGET_SAME_APP, 0)
       drawingArea.DragSourceSet(
               gdk.ModifierType(gdk.ALL_EVENTS_MASK),
               []gtk.TargetEntry{*target},
               gdk.ACTION_COPY,
       )

       drawingArea.Connect("drag-begin", func(
               da *gtk.DrawingArea,
               dc *gdk.DragContext,
       ) {
               fmt.Println("drag-begin")
       })

       gtk.Main()
}
