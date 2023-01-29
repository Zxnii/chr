#include "class.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

ConstantPoolEntry* read_constant_pool_entry(BinaryReader* reader) {
    ConstantPoolEntry* _entry = NULL;
    uint8_t tag = read_uint8(reader);

    switch (tag) {
        case 1: {
            ConstantUtf8Entry* entry = malloc(sizeof(ConstantUtf8Entry));

            entry->length = read_uint16_be(reader);
            entry->data = read_bytes(reader, entry->length);

            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 3: {
            ConstantIntegerEntry* entry = malloc(sizeof(ConstantIntegerEntry));

            entry->value = read_uint32_be(reader);
            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 4: {
            ConstantFloatEntry* entry = malloc(sizeof(ConstantFloatEntry));

            // todo: floating point numbers in bin-reader
            entry->value = read_uint32_be(reader);
            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 5: {
            ConstantLongEntry* entry = malloc(sizeof(ConstantLongEntry));

            entry->value = read_uint64_be(reader);
            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 6: {
            ConstantDoubleEntry* entry = malloc(sizeof(ConstantDoubleEntry));

            entry->value = read_uint64_be(reader);
            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 7:
        case 8: {
            ConstantClassEntry* entry = malloc(sizeof(ConstantClassEntry));

            entry->name_index = read_uint16_be(reader);
            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 9:
        case 10:
        case 11:
        case 12: {
            ConstantNameAndTypeEntry* entry = malloc(sizeof(ConstantNameAndTypeEntry));

            entry->name_index = read_uint16_be(reader);
            entry->descriptor_index = read_uint16_be(reader);

            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 15: {
            ConstantMethodHandleEntry* entry = malloc(sizeof(ConstantMethodHandleEntry));

            entry->reference_kind = read_uint8(reader);
            entry->reference_index = read_uint16_be(reader);

            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 16: {
            ConstantMethodTypeEntry* entry = malloc(sizeof(ConstantMethodTypeEntry));

            entry->descriptor_index = read_uint16_be(reader);
            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 17:
        case 18: {
            ConstantInvokeDynamicEntry* entry = malloc(sizeof(ConstantInvokeDynamicEntry));

            entry->bootstrap_method_attr_index = read_uint16_be(reader);
            entry->name_and_type_index = read_uint16_be(reader);

            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        case 19:
        case 20: {
            ConstantPackageEntry* entry = malloc(sizeof(ConstantPackageEntry));

            entry->name_index = read_uint16_be(reader);
            _entry = (ConstantPoolEntry*) entry;

            break;
        }
        default: {
            reader_seek(reader, -1);
            return NULL;
        }
    }

    if (_entry != NULL) {
        _entry->tag = tag;
    }

    return _entry;
}

AttributeInfo* read_attribute(BinaryReader* reader) {
    AttributeInfo* attribute = malloc(sizeof(attribute));

    attribute->attribute_name_index = read_uint16_be(reader);
    attribute->attribute_length = read_uint32_be(reader);
    attribute->info = read_bytes(reader, attribute->attribute_length);

    return attribute;
}

ClassHeader* read_class(BinaryReader* reader) {
    if (reader == NULL) {
        return NULL;
    }

    if (strncmp(read_bytes(reader, 4), "\xCA\xFE\xBA\xBE", 4) != 0) {
        return NULL;
    }

    ClassHeader* header = malloc(sizeof(ClassHeader));

    header->minor_version = read_uint16_be(reader);
    header->major_version = read_uint16_be(reader);

    header->constant_pool_count = read_uint16_be(reader);
    header->constant_pool = malloc(header->constant_pool_count * sizeof(ConstantPoolEntry*));

    header->constant_pool[0] = NULL;

    for (int i = 1; i < header->constant_pool_count; i++) {
        header->constant_pool[i] = read_constant_pool_entry(reader);
    }

    header->access_flags = read_uint16_be(reader);

    header->this_class = read_uint16_be(reader);
    header->super_class = read_uint16_be(reader);

    header->interface_count = read_uint16_be(reader);
    header->interfaces = malloc(header->interface_count * 2);

    for (int i = 0; i < header->interface_count; i++) {
        header->interfaces[i] = read_uint16_be(reader);
    }

    header->field_count = read_uint16_be(reader);
    header->fields = malloc(header->field_count * sizeof(FieldInfo));

    for (int i = 0; i < header->field_count; i++) {
        FieldInfo* field = malloc(sizeof(FieldInfo));

        field->access_flags = read_uint16_be(reader);
        field->name_index = read_uint16_be(reader);
        field->descriptor_index = read_uint16_be(reader);
        field->attribute_count = read_uint16_be(reader);
        field->attributes = malloc(field->attribute_count * sizeof(AttributeInfo));

        for (int j = 0; j < field->attribute_count; j++) {
            field->attributes[j] = read_attribute(reader);
        }

        header->fields[i] = field;
    }

    header->method_count = read_uint16_be(reader);
    header->methods = malloc(header->method_count * sizeof(MethodInfo));

    for (int i = 0; i < header->method_count; i++) {
        MethodInfo* method = malloc(sizeof(MethodInfo));

        method->access_flags = read_uint16_be(reader);
        method->name_index = read_uint16_be(reader);
        method->descriptor_index = read_uint16_be(reader);
        method->attribute_count = read_uint16_be(reader);
        method->attributes = malloc(method->attribute_count * sizeof(AttributeInfo));

        for (int j = 0; j < method->attribute_count; j++) {
            method->attributes[j] = read_attribute(reader);
        }

        header->methods[i] = method;
    }

    header->attribute_count = read_uint16_be(reader);
    header->attributes = malloc(header->attribute_count * sizeof(AttributeInfo));

    for (int i = 0; i < header->attribute_count; i++) {
        header->attributes[i] = read_attribute(reader);
    }

    ConstantClassEntry* this_class_entry = (ConstantClassEntry*) header->constant_pool[header->this_class];
    ConstantClassEntry* super_class_entry = (ConstantClassEntry*) header->constant_pool[header->super_class];

    ConstantUtf8Entry* this_class_name_entry = (ConstantUtf8Entry*) header->constant_pool[this_class_entry->name_index];
    ConstantUtf8Entry* super_class_name_entry = (ConstantUtf8Entry*) header->constant_pool[super_class_entry->name_index];

    header->class_name = malloc(this_class_name_entry->length + 1);
    header->super_name = malloc(super_class_name_entry->length + 1);

    header->class_name[this_class_name_entry->length] = '\0';
    header->super_name[super_class_name_entry->length] = '\0';

    memcpy(header->class_name, this_class_name_entry->data, this_class_name_entry->length);
    memcpy(header->super_name, super_class_name_entry->data, super_class_name_entry->length);

    return header;
}

ClassHeader* read_class_from_file(char* path) {
    BinaryReader* reader = new_reader_from_file(path);
    ClassHeader* header = read_class(reader);

    free_reader(reader);

    return header;
}

ClassHeader* read_class_from_data(uint8_t* data, size_t length) {
    BinaryReader* reader = new_reader(data, length);
    ClassHeader* header = read_class(reader);

    free_reader(reader);

    return header;
}

void free_class(ClassHeader* header) {
    for (int i = 0; i < header->constant_pool_count; i++) {
        void* entry = header->constant_pool[i];

        if (entry != NULL) {
            free(entry);
        }
    }

    free(header->constant_pool);
    free(header->interfaces);

    for (int i = 0; i < header->field_count; i++) {
        void* entry = header->fields[i];

        if (entry != NULL) {
            free(entry);
        }
    }

    free(header->fields);

    for (int i = 0; i < header->method_count; i++) {
        void* entry = header->methods[i];

        if (entry != NULL) {
            free(entry);
        }
    }

    free(header->methods);
    free(header->attributes);
    free(header->class_name);
    free(header->super_name);
    free(header);
}