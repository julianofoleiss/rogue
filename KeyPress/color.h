#ifndef COLOR_H
#define	COLOR_H

//http://misc.flogisoft.com/bash/tip_colors_and_formatting
//https://github.com/shiena/ansicolor/blob/master/README.md

#define ATTRIB_CODE_BOLD    "\e[1m"
#define ATTRIB_CODE_DIM     "\e[2m"
#define ATTRIB_CODE_UNDER   "\e[4m"
#define ATTRIB_CODE_BLINK   "\e[5m"
#define ATTRIB_CODE_REVERSE "\e[7m"
#define ATTRIB_CODE_HIDDEN  "\e[8m"
#define ATTRIB_CODE_OFF     "\x1b[0m"

#define COLOR_CODE_KNRM  "\x1B[0m"
#define COLOR_CODE_KRED  "\e[31m"
#define COLOR_CODE_KGRN  "\x1B[32m"
#define COLOR_CODE_KYEL  "\x1B[33m"
#define COLOR_CODE_KBLU  "\x1B[34m"
#define COLOR_CODE_KMAG  "\x1B[35m"
#define COLOR_CODE_KCYN  "\x1B[36m"
#define COLOR_CODE_KWHT  "\x1B[37m"
#define COLOR_CODE_RESET "\033[0m"
#define COLOR_CODE_LGRY "\x1b[90m"
#define COLOR_CODE_LRED "\x1b[91m"
#define COLOR_CODE_LGRN "\x1b[92m"
#define COLOR_CODE_LYEL "\x1b[93m"
#define COLOR_CODE_LBLU "\x1b[94m"
#define COLOR_CODE_LMAG "\x1b[95m"
#define COLOR_CODE_LCYN "\x1b[96m"
#define COLOR_CODE_LWHT "\x1b[97m"
        
#define BCOLOR_CODE_KNRM  "\x1B[49m"
#define BCOLOR_CODE_KBLK  "\e[40m"
#define BCOLOR_CODE_KRED  "\e[41m"
#define BCOLOR_CODE_KGRN  "\x1B[42m"
#define BCOLOR_CODE_KYEL  "\x1B[43m"
#define BCOLOR_CODE_KBLU  "\x1B[44m"
#define BCOLOR_CODE_KMAG  "\x1B[45m"
#define BCOLOR_CODE_KCYN  "\x1B[46m"
#define BCOLOR_CODE_KWHT  "\x1B[107m"
#define BCOLOR_CODE_LGRY "\x1b[100m"
#define BCOLOR_CODE_LRED "\x1b[101m"
#define BCOLOR_CODE_LGRN "\x1b[102m"
#define BCOLOR_CODE_LYEL "\x1b[103m"
#define BCOLOR_CODE_LBLU "\x1b[104m"
#define BCOLOR_CODE_LMAG "\x1b[105m"
#define BCOLOR_CODE_LCYN "\x1b[106m"
#define BCOLOR_CODE_LWHT "\x1b[107m"


#endif	/* COLOR_H */

