/*
 * Copyright (C) 2014 Sébastien Helleu <flashcode@flashtux.org>
 *
 * This file is part of WeeChat, the extensible chat client.
 *
 * WeeChat is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * WeeChat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WeeChat.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __WEECHAT_TRIGGER_H
#define __WEECHAT_TRIGGER_H 1

#include <regex.h>

#define weechat_plugin weechat_trigger_plugin
#define TRIGGER_PLUGIN_NAME "trigger"

enum t_trigger_option
{
    TRIGGER_OPTION_ENABLED = 0,        /* true if trigger is enabled        */
    TRIGGER_OPTION_HOOK,               /* hook (signal, modifier, ...)      */
    TRIGGER_OPTION_ARGUMENTS,          /* arguments for hook                */
    TRIGGER_OPTION_CONDITIONS,         /* conditions for trigger            */
    TRIGGER_OPTION_REGEX,              /* replace text with 1 or more regex */
    TRIGGER_OPTION_COMMAND,            /* command run if conditions are OK  */
    TRIGGER_OPTION_RETURN_CODE,        /* return code for hook callback     */
    /* number of trigger options */
    TRIGGER_NUM_OPTIONS,
};

enum t_trigger_hook_type
{
    TRIGGER_HOOK_SIGNAL = 0,
    TRIGGER_HOOK_HSIGNAL,
    TRIGGER_HOOK_MODIFIER,
    TRIGGER_HOOK_PRINT,
    TRIGGER_HOOK_TIMER,
    /* number of hook types */
    TRIGGER_NUM_HOOK_TYPES,
};

enum t_trigger_return_code
{
    TRIGGER_RC_OK = 0,
    TRIGGER_RC_OK_EAT,
    TRIGGER_RC_ERROR,
    /* number of return codes */
    TRIGGER_NUM_RETURN_CODES,
};

struct t_trigger_regex
{
    char *variable;                    /* the hashtable key used            */
    char *str_regex;                   /* regex to search for replacement   */
    regex_t *regex;                    /* compiled regex                    */
    char *replace;                     /* replacement text                  */
    char *replace_eval;                /* evaluatued replacement text       */
};

struct t_trigger
{
    /* user choices */
    char *name;                        /* trigger name                      */
    struct t_config_option *options[TRIGGER_NUM_OPTIONS];

    /* internal vars */

    /* regular expressions with their replacement text */
    int regex_count;                   /* number of regex                   */
    struct t_trigger_regex *regex;     /* array of regex                    */

    /* hooks */
    int hooks_count;                   /* number of hooks                   */
    struct t_hook **hooks;             /* array of hooks (signal, ...)      */
    unsigned long hook_count_cb;       /* number of calls made to callback  */
    unsigned long hook_count_cmd;      /* number of commands run in callback*/
    int hook_running;                  /* 1 if one hook callback is running */
    char *hook_print_buffers;          /* buffers (for hook_print only)     */

    /* links to other triggers */
    struct t_trigger *prev_trigger;    /* link to previous trigger          */
    struct t_trigger *next_trigger;    /* link to next trigger              */
};

extern struct t_weechat_plugin *weechat_trigger_plugin;
extern char *trigger_option_string[];
extern char *trigger_option_default[];
extern char *trigger_hook_type_string[];
extern char *trigger_hook_regex_default_var[];
extern int trigger_return_code[];
extern struct t_trigger *triggers;
extern struct t_trigger *last_trigger;
extern int triggers_count;
extern struct t_trigger *triggers_temp;
extern struct t_trigger *last_trigger_temp;

extern int trigger_search_option (const char *option_name);
extern int trigger_search_hook_type (const char *type);
extern struct t_trigger *trigger_search (const char *name);
extern struct t_trigger *trigger_search_with_option (struct t_config_option *option);
extern void trigger_set_regex (struct t_trigger *trigger);
extern void trigger_unhook (struct t_trigger *trigger);
extern void trigger_hook (struct t_trigger *trigger);
extern struct t_trigger *trigger_alloc (const char *name);
extern void trigger_add (struct t_trigger *trigger, struct t_trigger **triggers,
                         struct t_trigger **last_trigger);
extern struct t_trigger *trigger_new_with_options (const char *name,
                                                   struct t_config_option **options);
extern struct t_trigger *trigger_new (const char *name,
                                      const char *enabled,
                                      const char *hook,
                                      const char *arguments,
                                      const char *conditions,
                                      const char *replace,
                                      const char *command,
                                      const char *return_code);
extern int trigger_rename (struct t_trigger *trigger, const char *name);
extern void trigger_free (struct t_trigger *trigger);
extern void trigger_free_all ();

#endif /* __WEECHAT_TRIGGER_H */
