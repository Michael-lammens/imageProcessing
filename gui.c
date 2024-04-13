#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "video.h"

// Use a struct instead so we track previous processes. Make local to main
char *selected_image_filename = NULL;

// User clicks "Process". Open file and parse data. We need a goal for what we want to achieve here
void process_image(GtkWidget *widget, gpointer data) {
    // Check if an image has been selected
    if (selected_image_filename == NULL) {
        // No upload -> show error
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_CLOSE,
                                                   "Please select an image first!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        printf("Selected image filename: %s\n", selected_image_filename);

        unsigned char* image_data = readRawImage(selected_image_filename);
        if (image_data != NULL) {
            double average_intensity = calculateAverageIntensity(image_data);
            printf("Average pixel intensity: %f\n", average_intensity);
            // Do extra stuff here
            freeImageData(image_data);
        } else {
            perror("Error opening file");
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_CLOSE,
                                                       "Error opening file: %s not found",
                                                       selected_image_filename);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}

// Use this to init new struct for the new processing instance
/*
Struct process_data{
    file
    size
    dims
    // Get specific run data
    colored
    classification
    segmentation
    etc
}
*/
void file_selected(GtkFileChooserButton *chooser_button, gpointer data) {
    selected_image_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser_button));
}

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button %s clicked!\n", (char *)data);
}



static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *upload_image_button;
    GtkWidget *process_image_button;
    GtkWidget *box;
    GtkWidget *grid;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Image Processor");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create Header w/ Buttons + Title
    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Image Processor");
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);
    // BODY content grid:
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10); 
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);
    // Upload Image Button
    upload_image_button = gtk_file_chooser_button_new("Select Image", GTK_FILE_CHOOSER_ACTION_OPEN);
    g_signal_connect(upload_image_button, "file-set", G_CALLBACK(file_selected), NULL);
    gtk_grid_attach(GTK_GRID(grid), upload_image_button, 0, 0, 1, 1); 
    // Process Image Button
    process_image_button = gtk_button_new_with_label("Process");
    g_signal_connect(process_image_button, "clicked", G_CALLBACK(process_image), window);
    gtk_grid_attach(GTK_GRID(grid), process_image_button, 1, 0, 1, 1); 
    // Align grid to the top
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_START);

    gtk_widget_set_name(window, "main_window");
    
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    GError *error = NULL;
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "styles.css", &error);
    if (error) {
        g_warning("Error loading CSS: %s\n", error->message);
        g_clear_error(&error);
    }
    g_object_unref(provider);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("tester", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}