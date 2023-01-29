#ifndef CHR_CLASS_H
#define CHR_CLASS_H

#include <bin_reader/bin_reader.h>
#include <stdint.h>

// constant pool definitions

typedef struct {
    uint8_t tag;
    uint8_t* info;
} ConstantPoolEntry;

typedef struct {
    uint8_t tag;
    uint16_t length;
    const char* data;
} ConstantUtf8Entry;

typedef struct {
    uint8_t tag;
    uint32_t value;
} ConstantIntegerEntry;

typedef struct {
    uint8_t tag;
    // todo: float decoding
    uint32_t value;
} ConstantFloatEntry;

typedef struct {
    uint8_t tag;
    uint64_t value;
} ConstantLongEntry;

typedef struct {
    uint8_t tag;
    // todo: float decoding
    uint64_t value;
} ConstantDoubleEntry;

typedef struct {
    uint8_t tag;
    uint16_t name_index;
} ConstantClassEntry;

typedef struct {
    uint8_t tag;
    uint16_t string_index;
} ConstantStringEntry;

typedef struct {
    uint8_t tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
} ConstantFieldrefEntry;

typedef struct {
    uint8_t tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
} ConstantMethodrefEntry;

typedef struct {
    uint8_t tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
} ConstantInterfaceMethodrefEntry;

typedef struct {
    uint8_t tag;
    uint16_t name_index;
    uint16_t descriptor_index;
} ConstantNameAndTypeEntry;

typedef struct {
    uint8_t tag;
    uint8_t reference_kind;
    uint16_t reference_index;
} ConstantMethodHandleEntry;

typedef struct {
    uint8_t tag;
    uint16_t descriptor_index;
} ConstantMethodTypeEntry;

typedef struct {
    uint8_t tag;
    uint16_t bootstrap_method_attr_index;
    uint16_t name_and_type_index;
} ConstantDynamicEntry;

typedef struct {
    uint8_t tag;
    uint16_t bootstrap_method_attr_index;
    uint16_t name_and_type_index;
} ConstantInvokeDynamicEntry;

typedef struct {
    uint8_t tag;
    uint16_t name_index;
} ConstantModuleEntry;

typedef struct {
    uint8_t tag;
    uint16_t name_index;
} ConstantPackageEntry;

typedef struct {
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t* info;
} AttributeInfo;

typedef struct {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attribute_count;
    AttributeInfo** attributes;
} FieldInfo;

typedef struct {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attribute_count;
    AttributeInfo** attributes;
} MethodInfo;

typedef struct {
    uint16_t minor_version;
    uint16_t major_version;

    uint16_t constant_pool_count;
    ConstantPoolEntry** constant_pool;

    uint16_t access_flags;

    uint16_t this_class;
    uint16_t super_class;

    uint16_t interface_count;
    uint16_t* interfaces;

    uint16_t field_count;
    FieldInfo** fields;

    uint16_t method_count;
    MethodInfo** methods;

    uint16_t attribute_count;
    AttributeInfo** attributes;

    char* class_name;
    char* super_name;
} ClassHeader;

ClassHeader* read_class(BinaryReader* reader);
ClassHeader* read_class_from_file(char* file_path);
ClassHeader* read_class_from_data(uint8_t* data, size_t length);

AttributeInfo* read_attribute(BinaryReader* reader);
ConstantPoolEntry* read_constant_pool_entry(BinaryReader* reader);

void free_class(ClassHeader* header);

#endif
