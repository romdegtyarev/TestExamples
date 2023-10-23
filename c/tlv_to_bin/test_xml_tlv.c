#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/* Global constants */
#define XML_STRING_LEN_MAX    1024
#define TLV_STRING_LEN_MAX    XML_STRING_LEN_MAX * 2 +1
#define FILE_NAME_LEN_MAX     256
#define XML_TAG_LEN_MAX       256


/* Global Internal Enums */
typedef enum bool_T {
    FALSE,
    TRUE
} bool;

typedef enum xml_type_T {
    TEXT_XML,
    NUMERIC_XML
} xml_type;


/* Global Internal Struts */
typedef struct xml_file_tags_T {
char    start_tag[XML_TAG_LEN_MAX];
char    stop_tag[XML_TAG_LEN_MAX];
char    string_start_tag[XML_TAG_LEN_MAX];
char    string_stop_tag[XML_TAG_LEN_MAX];
char    numeric_start_tag[XML_TAG_LEN_MAX];
char    numeric_stop_tag[XML_TAG_LEN_MAX];
} xml_file_tags;

typedef struct tvl_entry_T {
xml_type    tag;
int         size;
char        data[TLV_STRING_LEN_MAX];
} tvl_entry;


/* Global Internal Variables */
xml_file_tags    XML_TAGS;
char           **XML_STRING_ARRAY_PTR;
tvl_entry       *TLV_ENTRY_PTR = NULL;


/* Functions */
void string2hexString(char *input, char *output) {
    /* Local variables */
    int    loop = 0;
    int    i = 0;

    /* Function Logic */
    while (input[loop] != '\0') {
        sprintf((char*)(output + i),"%02X", input[loop]);
        loop += 1;
        i += 2;
    }
    output[i++] = '\0';
}

int min_len(int a, int b) {
    if (a > b) {
        return b;
    }
    return a;
}

int save_tlv_to_file(char *tlv_file_name_PTR, int xml_string_count) {
    /* Local variables */
    FILE    *tlv_file_PTR = NULL;
    int      i;

    /* Function Logic */
    if (tlv_file_name_PTR == NULL) {
        printf("save_tlv_to_file: NULL file\n");
        return(-1);
    }
    if ((tlv_file_PTR = fopen(tlv_file_name_PTR, "w")) == NULL) {
        printf("save_tlv_to_file: Cannot open file \"%s\"\n", tlv_file_name_PTR);
        return(-1);
    }
    for (i = 0; i < xml_string_count; i++) {
        fprintf(tlv_file_PTR, "%x%x%s", TLV_ENTRY_PTR[i].tag, TLV_ENTRY_PTR[i].size, TLV_ENTRY_PTR[i].data);
    }
    if (fclose(tlv_file_PTR)) {
        printf("save_tlv_to_file: Cannot close file \"%s\"\n", tlv_file_name_PTR);
        return(-1);
    }
    return (0);
}

int xml_2_tlv(int xml_string_count) {
    /* Local variables */
    int      i;
    char    *substr_start_PTR = NULL;
    char    *substr_stop_PTR = NULL;
    char     tmp[XML_STRING_LEN_MAX] = {0};
    char     tmp_hex[TLV_STRING_LEN_MAX] = {0};
    int      tmp_len = 0;
    int      tmp_num = 0;

    /* Function Logic */
    TLV_ENTRY_PTR = (tvl_entry *)malloc(sizeof(tvl_entry) * xml_string_count);
    if (TLV_ENTRY_PTR == NULL) {
        printf("xml_2_tlv: Memory allocate error\n");
        return (-1);
    }

    for (i = 0; i < xml_string_count ; i++) {
        substr_start_PTR = strstr(XML_STRING_ARRAY_PTR[i], XML_TAGS.string_start_tag);
        if (substr_start_PTR) {
            substr_start_PTR = substr_start_PTR + strlen(XML_TAGS.string_start_tag);
            substr_stop_PTR = strstr(substr_start_PTR, XML_TAGS.string_stop_tag);
            if (substr_stop_PTR) {
                strncpy(tmp, substr_start_PTR, substr_stop_PTR - substr_start_PTR);
                tmp[substr_stop_PTR - substr_start_PTR] = '\0';
                string2hexString(tmp, tmp_hex);
                tmp_len = min_len(TLV_STRING_LEN_MAX, strlen(tmp_hex));
                printf("xml_2_tlv: tmp: %s tmp_hex: %s tmp_len: %d\n", tmp, tmp_hex, tmp_len);
                TLV_ENTRY_PTR[i].tag = TEXT_XML;
                TLV_ENTRY_PTR[i].size = tmp_len;
                strncpy(TLV_ENTRY_PTR[i].data, tmp_hex, tmp_len);
            } else {
                printf("xml_2_tlv: Warning file doesnt have stop tag, so skip\n");
                /* I dont really know what how to react to this situation */
            }
            continue;
        }

        substr_start_PTR = strstr(XML_STRING_ARRAY_PTR[i], XML_TAGS.numeric_start_tag);
        if (substr_start_PTR) {
            substr_start_PTR = substr_start_PTR + strlen(XML_TAGS.numeric_start_tag);
            substr_stop_PTR = strstr(substr_start_PTR, XML_TAGS.numeric_stop_tag);
            if (substr_stop_PTR) {
                strncpy(tmp, substr_start_PTR, substr_stop_PTR - substr_start_PTR);
                tmp[substr_stop_PTR - substr_start_PTR] = '\0';
                tmp_num = atoi(tmp);
                printf("xml_2_tlv: tmp: %s tmp_num: %d tmp_num: %x\n", tmp, tmp_num, tmp_num);
                TLV_ENTRY_PTR[i].tag = NUMERIC_XML;
                sprintf(TLV_ENTRY_PTR[i].data, "%x", tmp_num);
                TLV_ENTRY_PTR[i].size = strlen(TLV_ENTRY_PTR[i].data);
            } else {
                printf("xml_2_tlv: Warning file doesnt have stop tag, so skip\n");
                /* I dont really know what how to react to this situation */
            }
            continue;
        }

        printf("xml_2_tlv: Unknown tag: %s", XML_STRING_ARRAY_PTR[i]);
    }

    return (0);
}

int xml_reader(char *xml_file_name_PTR) {
    /* Local variables */
    FILE    *xml_file_PTR = NULL;
    int      xml_string_count;
    char     xml_string_STR[XML_STRING_LEN_MAX] = {0};
    bool     xml_start_stop = FALSE;
    int      i;
    int      ret_code;

    /* Function Logic */
    if (xml_file_name_PTR == NULL) {
        printf("xml_reader: NULL file\n");
        return(-1);
    }
    if ((xml_file_PTR = fopen(xml_file_name_PTR, "r")) == NULL) {
        printf("xml_reader: Cannot open file \"%s\"\n", xml_file_name_PTR);
        return(-1);
    }

    /* Count strings between <START> and </START> */
    xml_string_count = 0;
    while (!feof(xml_file_PTR)) {
        if (fgets(xml_string_STR, XML_STRING_LEN_MAX, xml_file_PTR)) {
            if ((xml_start_stop == TRUE) &&
                strcmp(xml_string_STR, XML_TAGS.stop_tag)) {
                xml_string_count++;
                continue;
            }
            if (!strcmp(xml_string_STR, XML_TAGS.start_tag)) {
                xml_start_stop = TRUE;
            } else if (!strcmp(xml_string_STR, XML_TAGS.stop_tag)) {
                xml_start_stop = FALSE;
                break;
            }
        }
    }
    printf("xml_reader: String count: %d xml_start_stop: %d\n", xml_string_count, xml_start_stop);
    if (xml_start_stop) {
        printf("xml_reader: Warning file doesnt have stop tag\n");
        /* I dont really know what how to react to this situation */
    }


    /* Prepare array for data */
    XML_STRING_ARRAY_PTR = (char **)malloc(sizeof(char*) * xml_string_count);
    if (XML_STRING_ARRAY_PTR == NULL) {
        printf("xml_reader: Memory allocate error\n");
        return(-1);
    }
    for (i = 0; i < xml_string_count ; i++) {
        XML_STRING_ARRAY_PTR[i] = (char *)malloc(sizeof(char) * XML_STRING_LEN_MAX);
        if (XML_STRING_ARRAY_PTR[i] == NULL) {
            printf("xml_reader: Memory allocate error\n");
            return(-1);
        }
        memset(XML_STRING_ARRAY_PTR[i], 0, XML_STRING_LEN_MAX);
    }

    /* Save data to array */
    i = 0;
    ret_code = fseek(xml_file_PTR, 0, SEEK_SET);
    while (!feof(xml_file_PTR) &&
           (i < xml_string_count) &&
           !ret_code) {

        if (fgets(xml_string_STR, XML_STRING_LEN_MAX, xml_file_PTR)) {
            if ((xml_start_stop == TRUE) &&
                strcmp(xml_string_STR, XML_TAGS.stop_tag)) {
                strncpy(XML_STRING_ARRAY_PTR[i], xml_string_STR, min_len(XML_STRING_LEN_MAX, strlen(xml_string_STR)));
                i++;
                continue;
            }
            if (!strcmp(xml_string_STR, XML_TAGS.start_tag)) {
                xml_start_stop = TRUE;
            } else if (!strcmp(xml_string_STR, XML_TAGS.stop_tag)) {
                xml_start_stop = FALSE;
                break;
            }
        }
    }
    printf("xml_reader: String count: %d ret_code: %d\n", i, ret_code);

    if (fclose(xml_file_PTR)) {
        printf("xml_reader: Cannot close file \"%s\"\n", xml_file_name_PTR);
        return(-1);
    }

    return (xml_string_count);
}

void init() {
    /* Function Logic */
    memset(&XML_TAGS, 0, sizeof(xml_file_tags));
    strncpy(XML_TAGS.start_tag,         "<START>\n",  min_len(XML_TAG_LEN_MAX, strlen("<START>\n")));
    strncpy(XML_TAGS.stop_tag,          "</START>\n", min_len(XML_TAG_LEN_MAX, strlen("</START>\n")));
    strncpy(XML_TAGS.string_start_tag,  "<text>",     min_len(XML_TAG_LEN_MAX, strlen("<text>")));
    strncpy(XML_TAGS.string_stop_tag,   "</text>",    min_len(XML_TAG_LEN_MAX, strlen("</text>")));
    strncpy(XML_TAGS.numeric_start_tag, "<numeric>",  min_len(XML_TAG_LEN_MAX, strlen("<numeric>")));
    strncpy(XML_TAGS.numeric_stop_tag,  "</numeric>", min_len(XML_TAG_LEN_MAX, strlen("</numeric>")));
}

/*
 * Open XML File
 * Count strings between <START> and </START>
 * Read <START>
 * Read N string
 * Parse N string
 * Convert N string
 * Save N string
 * Close XML File
 * Open TLV file
 * Write data
 * Close TLV file
 */
int main(int argc, char **argv) {
    /* Local variables */
    char    file_name_STR[FILE_NAME_LEN_MAX] = {0};
    int     xml_string_count;
    int     i = 0;

    /* Function Logic */
    printf("main: Start\n");
    init();
    printf("main: Enter XML File name: ");
    scanf("%255s", file_name_STR);
    xml_string_count = xml_reader(file_name_STR);
    xml_2_tlv(xml_string_count);
    printf("main: Enter TLV File name: ");
    scanf("%255s", file_name_STR);
    save_tlv_to_file(file_name_STR, xml_string_count);
    for (i = 0; i < xml_string_count; i++) {
        free(XML_STRING_ARRAY_PTR[i]);
    }
    free(XML_STRING_ARRAY_PTR);
    free(TLV_ENTRY_PTR);

    return (0);
}
