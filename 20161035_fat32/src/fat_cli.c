#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "fat32_driver.h"
#include "utils.h"

/* help and commands */
void print_help() {
    printf("Syntax : fat_cli <FAT disk> <command> <arguments>\n");
    printf("Commands available :\n");
    printf("* tree: displays a tree of folders and files\n");
    printf("* ls <path>: list the files in a folder\n");
    printf("* cat <path>: displays the contents of a file\n");
    printf("\n");
    printf("For Example :\n");
    printf("fat_cli path/to/the/disk.img ls /path/to/folder/\n");
}

/* also you can use auxiliary function for **tree** */

void log_tree(struct fat32_node *root, int depth) {
    const char *name = fat32_node_get_name(root);

    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) return;

    if (fat32_node_is_dir(root)) {
        printf("%*c> %s\n", 3 * depth, ' ', name);

        struct fat32_node_list* children = fat32_node_get_children(root);
        struct fat32_node_list* tmp = children;

        while(tmp) {
          log_tree(tmp->node, depth + 1);
          tmp = tmp->next;
        }

        fat32_node_list_free(children);
    }
    else {
      printf("%*c- %s\n", 3 * depth, ' ', name);
    }
    return;
}

/* "tree" command. */
int tree(char* fat_disk) {
    struct fat32_driver *driver = fat32_driver_new(fat_disk);
    struct fat32_node *root = fat32_driver_get_root_dir(driver);

    log_tree(root, 0);

    fat32_node_free(root);
    fat32_driver_free(driver);
    return EXIT_SUCCESS;
}

/* "ls" command. */
int ls(char* fat_disk, char* path) {
    struct fat32_driver *driver = fat32_driver_new(fat_disk);
    struct fat32_node *root = fat32_driver_get_root_dir(driver);

    struct fat32_node *node = fat32_node_get_path(root, path);

    if (node == NULL || !fat32_node_is_dir(node)) {
      fprintf(stderr, "is not a directory\n");
      return EXIT_FAILURE;
    }

    struct fat32_node_list* children = fat32_node_get_children(node);
    struct fat32_node_list* tmp = children;

    while (tmp) {
      printf("%s\n", fat32_node_get_name(tmp->node));
      tmp = tmp->next;
    }

    fat32_node_list_free(children);
    fat32_node_free(node);
    fat32_node_free(root);
    fat32_driver_free(driver);

    return EXIT_SUCCESS;
}

/* "cat" command. */
int cat(char* fat_disk, char* path) {
    struct fat32_driver *driver = fat32_driver_new(fat_disk);
    struct fat32_node *root = fat32_driver_get_root_dir(driver);
    struct fat32_node *node = fat32_node_get_path(root, path);

    if (node == NULL || fat32_node_is_dir(node)) {
      fprintf(stderr, "does not exist or is a directory\n");
      return EXIT_FAILURE;
    }
    
    fat32_node_read_to_fd(node, stdout);

    fat32_node_free(node);
    fat32_node_free(root);
    fat32_driver_free(driver);

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[2], "tree") == 0) {
        /* If the command is "tree" */
        return tree(argv[1]);
    }
    else if (argc == 4 && strcmp(argv[2], "ls") == 0) {
        /* If the command is "ls" */
        return ls(argv[1], argv[3]);
    }
    else if (argc == 4 && strcmp(argv[2], "cat") == 0) {
        /* If the command is "cat" */
        return cat(argv[1], argv[3]);
    }
    else {
        /* if command is unknown then we show how to use the
         * program. */
        print_help();
        return EXIT_FAILURE;
    }

}
