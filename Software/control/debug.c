#include "../inc/homeconnect.h"

int ft_debugger(void)
{
#ifdef DEBUG_ON
    return (1);
#else
    static int debug_enabled = -1;

    char    *env;

    if (debug_enabled)
    {
        env = getenv("DEBUG_ON");
        debug_enabled = (env != NULL);
    }
    return (debug_enabled);
#endif
}


// static int debug_enabled = -1; 

// int ft_debugger(void)
// {
//     char    *env;

//     if (debug_enabled == -1)
//     {
//         env = getenv("DEBUG_ON");
//         if (env == NULL)
//             debug_enabled = 0;
//         else
//             debug_enabled = 1;
//     }
//     return (debug_enabled);
// }