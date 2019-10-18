#include <gtk/gtk.h>

static void activate(
  GtkApplication* app, 
  gpointer user_data) 
{
  GtkWidget *window;
  GtkWidget *container;
  GtkWidget *label1;
  GtkStyleContext *context;

  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "Prueba1.css", NULL);
  gtk_style_context_add_provider_for_screen(
    gdk_screen_get_default(),
    GTK_STYLE_PROVIDER(cssProvider),
    GTK_STYLE_PROVIDER_PRIORITY_USER);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

  container = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), container);

  label1 = gtk_label_new("Pinche boquita-patito");
  gtk_fixed_put(GTK_FIXED(container), label1, 100, 100);

  context = gtk_widget_get_style_context(GTK_WIDGET(label1));
  gtk_style_context_add_class(GTK_STYLE_CONTEXT(context), "tag1");

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