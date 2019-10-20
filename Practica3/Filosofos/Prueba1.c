#include <gtk/gtk.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

int activo = 0;
GtkWidget *label1;

#define N 5 
#define RADIO 180
GtkWidget **imagenes;

static void click(
  GtkWidget *widget,
  gpointer user_data
) 
{
  activo ^= 1;

  GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(label1));

  if (activo)
    gtk_style_context_add_class(GTK_STYLE_CONTEXT(context), "tag2");
  else
    gtk_style_context_remove_class(GTK_STYLE_CONTEXT(context), "tag2");
}

static void activate(
  GtkApplication* app, 
  gpointer user_data) 
{
  GtkWidget *window, *container, *button, *buttonBox, *image;
  GtkStyleContext *context;

  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "Prueba1.css", NULL);
  gtk_style_context_add_provider_for_screen(
    gdk_screen_get_default(),
    GTK_STYLE_PROVIDER(cssProvider),
    GTK_STYLE_PROVIDER_PRIORITY_USER);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  container = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), container);

  image = gtk_image_new_from_file("pensando.gif");
  context = gtk_widget_get_style_context(GTK_WIDGET(image));
  gtk_style_context_add_class(GTK_STYLE_CONTEXT(context), "fork");
  gtk_fixed_put(GTK_FIXED(container), image, 0, 0);

  label1 = gtk_label_new("F1");
  gtk_fixed_put(GTK_FIXED(container), label1, 200, 100);

  buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_fixed_put(GTK_FIXED(container), buttonBox, 100, 300);

  button = gtk_button_new_with_label("Che");
  g_signal_connect(button, "clicked", G_CALLBACK(click), NULL);
  gtk_container_add(GTK_CONTAINER(buttonBox), button);

  context = gtk_widget_get_style_context(GTK_WIDGET(label1));
  gtk_style_context_add_class(GTK_STYLE_CONTEXT(context), "tag1");

  int i, j, idx;
  int angulo = (int) (2 * G_PI / (double) N);
  char *nombreImagenes[] = { "pensando.gif", "hambriento.gif", "comiendo.gif" };
  char idImagen[15];
  char estilo[90];

  imagenes = malloc(sizeof(GtkWidget *) * N * 3);
  for (i = 0; i < N; i++) {
    for (j = 0; j < 3; j++) {
      idx = i * 3 + j;
      imagenes[idx] = gtk_image_new_from_file(nombreImagenes[j]);

      sprintf(idImagen, "imagen-%d-%d", i, j);
      gtk_widget_set_name(imagenes[idx], idImagen);

      gtk_fixed_put(GTK_FIXED(container), imagenes[idx], 0, 0);

      GtkCssProvider *proveedor = gtk_css_provider_new();
      sprintf(
        estilo, 
        "#imagen-%d-%d { -gtk-icon-transform: translate(%dpx, %dpx) scale(0.15); opacity: 1; }",
        i, j, (int) (cos(angulo * i) * RADIO), (int) (sin(angulo * i) * RADIO));
      gtk_css_provider_load_from_data(proveedor, estilo, strlen(estilo), NULL);
      gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(proveedor),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
  }

  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new(
    "org.ggzor.collab.paquetito.filosofos", 
    G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}