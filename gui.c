#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>

// Global variable to store the selected image filename
char *selected_image_filename = NULL;

// Function to handle "Process" button click event
void process_image(GtkWidget *widget, gpointer data) {
    // Check if an image has been selected
    if (selected_image_filename == NULL) {
        // No image selected, show an error message
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_CLOSE,
                                                   "Please select an image first!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Print the selected image filename for debugging
        printf("Selected image filename: %s\n", selected_image_filename);
        
        // Construct the full path to the image file
        char full_path[512]; 
        snprintf(full_path, sizeof(full_path), "./output_images_raw/%s", selected_image_filename);

        // Check if the file exists
        if (access(selected_image_filename, F_OK) == 0) {
            // File exists, call the video processing function with the selected image filename
            char command[256];
            snprintf(command, sizeof(command), "./video %s", full_path);
            system(command);
        } else {
            // File does not exist, show an error message
             // File does not exist or there was an error accessing it
            if (errno == ENOENT) {
                fprintf(stderr, "File %s does not exist.\n", full_path);
            } else {
                perror("Error accessing file");
            }



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

// Function to handle file selection
void file_selected(GtkFileChooserButton *chooser_button, gpointer data) {
    selected_image_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser_button));
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image Processor");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    // Create the "Select Image" button
    GtkWidget *select_button = gtk_file_chooser_button_new("Select Image", GTK_FILE_CHOOSER_ACTION_OPEN);
    
    // Connect the "Select Image" button file selection event to a callback function
    g_signal_connect(select_button, "file-set", G_CALLBACK(file_selected), NULL);

    // Create the "Process" button
    GtkWidget *process_button = gtk_button_new_with_label("Process");

    // Connect the "Process" button click event to a callback function
    g_signal_connect(process_button, "clicked", G_CALLBACK(process_image), window);

    // Create a vertical box container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Add the "Select Image" button and "Process" button to the vertical box
    gtk_box_pack_start(GTK_BOX(vbox), select_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), process_button, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    // Free selected_image_filename
    g_free(selected_image_filename);

    return 0;
}