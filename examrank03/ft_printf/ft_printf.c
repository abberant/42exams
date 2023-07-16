#include <stdarg.h>
#include <unistd.h>

void    _putstr(char *str, int *len)
{
    if (!str)
        str = "(null)";
    while (*str)
    {
        *len += write(1, str, 1);
        str++;
    }
}

void _putdigit(long long int nbr, int base, int *len)
{
    char *layout;

    layout = "0123456789abcdef";
    if (nbr < 0)
    {
        nbr *= -1;
        *len += write(1, "-", 1);
    }
    if (nbr >= base)
        _putdigit(nbr / base, base, len);
    *len += write(1, &layout[nbr % base], 1);
}

int ft_printf(const char *format, ...)
{
    int len;
    va_list ap;

    len = 0;
    va_start(ap, format);
    while (*format)
    {
        if ((*format == '%') && *(format + 1))
        {
            format++;
            if (*format == 's')
                _putstr(va_arg(ap, char *), &len);
            else if (*format == 'd')
                _putdigit((long long int)va_arg(ap, int), 10 , &len);
            else if (*format == 'x')
                _putdigit((long long int)va_arg(ap, unsigned int), 16, &len);
        }
        else
            len += write(1, format, 1);
        format++;
    }
    va_end(ap);
    return (len);
}
