#include <chr/class.h>

#include <stdio.h>

int main(int argc, char **argv) {
    if (argc <= 1) {
        fputs("Usage: classinfo <class_file>", stderr);

        return 1;
    }

    puts("Reading class");

    ClassHeader *header = read_class_from_file(
            argv[1]);

    if (header != NULL) {
        puts("Valid class");
    } else {
        fputs("Invalid class", stderr);
    }

    printf("Class name: %s\n"
           "Super class: %s\n"
           "Access flags: 0x%04x\n"
           "Fields: %hu\n"
           "Methods: %hu\n",
           header->class_name,
           header->super_name,
           header->access_flags,
           header->field_count,
           header->method_count);

    free_class(header);

    return 0;
}