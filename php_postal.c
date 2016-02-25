/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_postal.h"

#include <libpostal/libpostal.h>

/* If you declare any globals in php_postal.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(postal)
*/

/* True global resources - no need for thread safety here */
static int le_postal;


zend_class_entry expand_ce;
zend_class_entry *expand_class_entry_ptr;

zend_class_entry parser_ce;
zend_class_entry *parser_class_entry_ptr;

/* {{{ expand_methods[]
 * 
 * Every user visible method must have an entry in expand_methods
 */
static const zend_function_entry expand_methods[] = {
  PHP_ME(Expand, expand_address, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END /* Must be the last line in expand_methods[] */
};
 
/* }}} */

/* {{{ parser_methods[]
 * 
 * Every user visible method must have an entry in parser_methods
 */
static const zend_function_entry parser_methods[] = {
  PHP_ME(Parser, parse_address, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END /* Must be the last line in expand_methods[] */
};
 
/* }}} */

/* {{{ postal_module_entry
 */
zend_module_entry postal_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"postal",
	NULL,
	PHP_MINIT(postal),
	PHP_MSHUTDOWN(postal),
	NULL,
	NULL,
	PHP_MINFO(postal),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_POSTAL_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_POSTAL
ZEND_GET_MODULE(postal)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("postal.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_postal_globals, postal_globals)
    STD_PHP_INI_ENTRY("postal.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_postal_globals, postal_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_postal_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_postal_init_globals(zend_postal_globals *postal_globals)
{
	postal_globals->global_value = 0;
	postal_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(postal)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
    if (!libpostal_setup() || !libpostal_setup_language_classifier() || !libpostal_setup_parser()) {
        return FAILURE;
    }

    REGISTER_LONG_CONSTANT("ADDRESS_NONE", ADDRESS_NONE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_ANY", ADDRESS_ANY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_NAME", ADDRESS_NAME, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_HOUSE_NUMBER", ADDRESS_HOUSE_NUMBER, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_STREET", ADDRESS_STREET, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_UNIT", ADDRESS_UNIT, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_LOCALITY", ADDRESS_LOCALITY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_ADMIN1", ADDRESS_ADMIN1, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_ADMIN2", ADDRESS_ADMIN2, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_ADMIN3", ADDRESS_ADMIN3, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_ADMIN4", ADDRESS_ADMIN4, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_ADMIN_OTHER", ADDRESS_ADMIN_OTHER, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_COUNTRY", ADDRESS_COUNTRY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_POSTAL_CODE", ADDRESS_POSTAL_CODE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_NEIGHBORHOOD", ADDRESS_NEIGHBORHOOD, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("ADDRESS_ALL", ADDRESS_ALL, CONST_CS | CONST_PERSISTENT);


    INIT_CLASS_ENTRY(expand_ce, "Postal\\Expand", expand_methods);
    expand_class_entry_ptr = zend_register_internal_class(&expand_ce TSRMLS_CC);

    INIT_CLASS_ENTRY(parser_ce, "Postal\\Parser", parser_methods);
    parser_class_entry_ptr = zend_register_internal_class(&parser_ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(postal)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/

    libpostal_teardown();
    libpostal_teardown_language_classifier();
    libpostal_teardown_parser();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(postal)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "postal support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


PHP_METHOD(Expand, expand_address) {
    char *address;
    int address_len;
    HashTable *php_options = NULL;
    zval **val;
    zval *ret;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|h", &address, &address_len, &php_options) == FAILURE) {
        return;
    }

    normalize_options_t options = get_libpostal_default_options();

    size_t num_languages = 0;
    char **languages = NULL;

    if (php_options != NULL) {
        #define LANGUAGES_KEY "languages"

        if (zend_hash_find(php_options, LANGUAGES_KEY, strlen(LANGUAGES_KEY) + 1, (void **)&val) == SUCCESS) {
            if (Z_TYPE_PP(val) != IS_ARRAY) {
                RETURN_NULL();
            }
            size_t num_php_langs = zend_hash_num_elements(Z_ARRVAL_PP(val));
            languages = malloc(sizeof(char *) * num_php_langs);

            HashPosition pos;
            zval **php_lang;

            zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP(val), &pos);

            while (zend_hash_get_current_data_ex(Z_ARRVAL_PP(val), (void **)&php_lang, &pos) == SUCCESS) {

                if (Z_TYPE_PP(php_lang) != IS_STRING) {
                    continue;
                }
                size_t lang_len = Z_STRLEN_PP(php_lang);
                if (lang_len > MAX_LANGUAGE_LEN) {
                    continue;
                }
                char *lang = strndup(Z_STRVAL_PP(php_lang), lang_len);

                languages[num_languages++] = lang;

                zend_hash_move_forward_ex(Z_ARRVAL_PP(val), &pos);
            }

            if (num_languages > 0) {
                options.languages = languages;
                options.num_languages = num_languages;
            }
        }

        #define ADDRESS_COMPONENTS_KEY "address_components"

        if (zend_hash_find(php_options, ADDRESS_COMPONENTS_KEY, strlen(ADDRESS_COMPONENTS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.address_components = (uint16_t)Z_LVAL_PP(val);
        }

        #define LATIN_ASCII_KEY "latin_ascii"
        if (zend_hash_find(php_options, LATIN_ASCII_KEY, strlen(LATIN_ASCII_KEY) + 1, (void **) &val) == SUCCESS) {
            options.latin_ascii = (bool)Z_LVAL_PP(val);
        }

        #define TRANSLITERATE_KEY "transliterate"
        if (zend_hash_find(php_options, TRANSLITERATE_KEY, strlen(TRANSLITERATE_KEY) + 1, (void **) &val) == SUCCESS) {
            options.transliterate = (bool)Z_LVAL_PP(val);
        }

        #define STRIP_ACCENTS_KEY "strip_accents"
        if (zend_hash_find(php_options, STRIP_ACCENTS_KEY, strlen(STRIP_ACCENTS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.strip_accents = (bool)Z_LVAL_PP(val);
        }

        #define DECOMPOSE_KEY "decompose"
        if (zend_hash_find(php_options, DECOMPOSE_KEY, strlen(DECOMPOSE_KEY) + 1, (void **) &val) == SUCCESS) {
            options.decompose = (bool)Z_LVAL_PP(val);
        }

        #define LOWERCASE_KEY "lowercase"
        if (zend_hash_find(php_options, LOWERCASE_KEY, strlen(LOWERCASE_KEY) + 1, (void **) &val) == SUCCESS) {
            options.lowercase = (bool)Z_LVAL_PP(val);
        }

        #define TRIM_STRING_KEY "trim_string"
        if (zend_hash_find(php_options, TRIM_STRING_KEY, strlen(TRIM_STRING_KEY) + 1, (void **) &val) == SUCCESS) {
            options.trim_string = (bool)Z_LVAL_PP(val);
        }

        #define DROP_PARENTHETICALS_KEY "drop_parentheticals"
        if (zend_hash_find(php_options, DROP_PARENTHETICALS_KEY, strlen(DROP_PARENTHETICALS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.drop_parentheticals = (bool)Z_LVAL_PP(val);
        }

        #define REPLACE_NUMERIC_HYPHENS_KEY "replace_numeric_hyphens"
        if (zend_hash_find(php_options, REPLACE_NUMERIC_HYPHENS_KEY, strlen(REPLACE_NUMERIC_HYPHENS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.replace_numeric_hyphens  = (bool)Z_LVAL_PP(val);
        }

        #define DELETE_NUMERIC_HYPHENS_KEY "delete_numeric_hyphens"
        if (zend_hash_find(php_options, DELETE_NUMERIC_HYPHENS_KEY, strlen(DELETE_NUMERIC_HYPHENS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.delete_numeric_hyphens  = (bool)Z_LVAL_PP(val);
        }

        #define SPLIT_ALPHA_FROM_NUMERIC_KEY "split_alpha_from_numeric"
        if (zend_hash_find(php_options, SPLIT_ALPHA_FROM_NUMERIC_KEY, strlen(SPLIT_ALPHA_FROM_NUMERIC_KEY) + 1, (void **) &val) == SUCCESS) {
            options.split_alpha_from_numeric = (bool)Z_LVAL_PP(val);
        }

        #define REPLACE_WORD_HYPHENS_KEY "replace_word_hyphens"
        if (zend_hash_find(php_options, REPLACE_WORD_HYPHENS_KEY, strlen(REPLACE_WORD_HYPHENS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.replace_word_hyphens = (bool)Z_LVAL_PP(val);
        }

        #define DELETE_WORD_HYPHENS_KEY "delete_word_hyphens"
        if (zend_hash_find(php_options, DELETE_WORD_HYPHENS_KEY, strlen(DELETE_WORD_HYPHENS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.delete_word_hyphens = (bool)Z_LVAL_PP(val);
        }

        #define DELETE_FINAL_PERIODS_KEY "delete_final_periods"
        if (zend_hash_find(php_options, DELETE_FINAL_PERIODS_KEY, strlen(DELETE_FINAL_PERIODS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.delete_final_periods = (bool)Z_LVAL_PP(val);
        }

        #define DELETE_ACRONYM_PERIODS_KEY "delete_acronym_periods"
        if (zend_hash_find(php_options, DELETE_ACRONYM_PERIODS_KEY, strlen(DELETE_ACRONYM_PERIODS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.delete_acronym_periods = (bool)Z_LVAL_PP(val);
        }

        #define DROP_ENGLISH_POSSESSIVES_KEY "drop_english_possessives"
        if (zend_hash_find(php_options, DROP_ENGLISH_POSSESSIVES_KEY, strlen(DROP_ENGLISH_POSSESSIVES_KEY) + 1, (void **) &val) == SUCCESS) {
            options.drop_english_possessives = (bool)Z_LVAL_PP(val);
        }

        #define DELETE_APOSTROPHES_KEY "delete_apostrophes"
        if (zend_hash_find(php_options, DELETE_APOSTROPHES_KEY, strlen(DELETE_APOSTROPHES_KEY) + 1, (void **) &val) == SUCCESS) {
            options.delete_apostrophes = (bool)Z_LVAL_PP(val);
        }

        #define EXPAND_NUMEX_KEY "expand_numex"
        if (zend_hash_find(php_options, EXPAND_NUMEX_KEY, strlen(EXPAND_NUMEX_KEY) + 1, (void **) &val) == SUCCESS) {
            options.expand_numex = (bool)Z_LVAL_PP(val);
        }

        #define ROMAN_NUMERALS_KEY "roman_numerals"
        if (zend_hash_find(php_options, ROMAN_NUMERALS_KEY, strlen(ROMAN_NUMERALS_KEY) + 1, (void **) &val) == SUCCESS) {
            options.roman_numerals = (bool)Z_LVAL_PP(val);
        }
    }

    size_t num_expansions;
    char **expansions = expand_address(address, options, &num_expansions);

    ALLOC_INIT_ZVAL(ret);
    array_init_size(ret, num_expansions);
    int copy = 1;
    for (size_t i = 0; i < num_expansions; i++) {
        add_index_string(ret, (int)i, expansions[i], copy);
    }

    expansion_array_destroy(expansions, num_expansions);

    if (num_languages > 0) {
        for (size_t i = 0; i < num_languages; i++) {
            free(languages[i]);
        }
    }

    if (languages != NULL) {
        free(languages);
    }

    copy = 0;
    int destruct = 0;
    RETURN_ZVAL(ret, copy, destruct);

}

PHP_METHOD(Parser, parse_address) {
    char *address;
    int address_len;
    zval *ret;
    zval *component;
    HashTable *php_options = NULL;
    zval **val;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|h", &address, &address_len, &php_options) == FAILURE) {
        RETURN_NULL();
    }

    address_parser_options_t options = get_libpostal_address_parser_default_options();
    
    char *language = NULL;
    char *country = NULL;

    if (php_options != NULL) {
        #define LANGUAGE_KEY "language"

        if (zend_hash_find(php_options, LANGUAGE_KEY, strlen(LANGUAGE_KEY) + 1, (void **)&val) == SUCCESS && Z_TYPE_PP(val) == IS_STRING) {
            language = strndup(Z_STRVAL_PP(val), Z_STRLEN_PP(val));
            options.language = language;
        }

        #define COUNTRY_KEY "language"

        if (zend_hash_find(php_options, COUNTRY_KEY, strlen(COUNTRY_KEY) + 1, (void **)&val) == SUCCESS && Z_TYPE_PP(val) == IS_STRING) {
            country = strndup(Z_STRVAL_PP(val), Z_STRLEN_PP(val));
            options.country = country;
        }
    }

    size_t num_expansions;
    address_parser_response_t *response = parse_address(address, options);

    if (language != NULL) {
        free(language);
    }

    if (country != NULL) {
        free(country);
    }

    if (response != NULL) {

        ALLOC_INIT_ZVAL(ret);
        array_init_size(ret, response->num_components);
        int copy = 1;

        #define LABEL_KEY "label"
        #define COMPONENT_KEY "component"

        for (size_t i = 0; i < response->num_components; i++) {
            ALLOC_INIT_ZVAL(component);
            array_init(component);
            add_assoc_string(component, LABEL_KEY, response->labels[i], copy);
            add_assoc_string(component, COMPONENT_KEY, response->components[i], copy);
            add_index_zval(ret, (int)i, component);
        }

        address_parser_response_destroy(response);

        copy = 0;
        int destruct = 0;
        RETURN_ZVAL(ret, copy, destruct);
    } else {
        RETURN_NULL();
    }

}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
