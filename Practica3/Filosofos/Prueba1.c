#include <gtk/gtk.h>

int activo = 0;
GtkWidget *label1;

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


  gtk_icon_theme_add_resource_path(gtk_icon_theme_get_for_screen(gdk_screen_get_default()), ".");

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

  container = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), container);

  // image = gtk_image_new_from_icon_name("document-save-as", GTK_ICON_SIZE_LARGE_TOOLBAR);
  image = gtk_image_new_from_file("fork.png");
  context = gtk_widget_get_style_context(GTK_WIDGET(image));
  gtk_style_context_add_class(GTK_STYLE_CONTEXT(context), "fork");
  gtk_fixed_put(GTK_FIXED(container), image, 300, 300);

  label1 = gtk_label_new("F1");
  gtk_fixed_put(GTK_FIXED(container), label1, 100, 100);

  buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_fixed_put(GTK_FIXED(container), buttonBox, 100, 300);

  button = gtk_button_new_with_label("Che");
  g_signal_connect(button, "clicked", G_CALLBACK(click), NULL);
  gtk_container_add(GTK_CONTAINER(buttonBox), button);

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