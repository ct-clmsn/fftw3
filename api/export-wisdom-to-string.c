/*
 * Copyright (c) 2002 Matteo Frigo
 * Copyright (c) 2002 Steven G. Johnson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "api.h"

typedef struct {
     printer super;
     uint *cnt;
} P_cnt;

static void putchr_cnt(printer *p_, char c)
{
     P_cnt *p = (P_cnt *) p_;
     UNUSED(c);
     ++*p->cnt;
}

printer *X(mkprinter_cnt)(uint *cnt)
{
     P_cnt *p = (P_cnt *) X(mkprinter)(sizeof(P_cnt), putchr_cnt);
     p->cnt = cnt;
     *cnt = 0;
     return &p->super;
}

typedef struct {
     printer super;
     char *s;
} P_str;

static void putchr_str(printer *p_, char c)
{
     P_str *p = (P_str *) p_;
     *p->s++ = c;
     *p->s = 0;
}

printer *X(mkprinter_str)(char *s)
{
     P_str *p = (P_str *) X(mkprinter)(sizeof(P_str), putchr_str);
     p->s = s;
     *s = 0;
     return &p->super;
}

char *X(export_wisdom_to_string)(void)
{
     printer *p;
     planner *plnr = X(the_planner)();
     uint cnt;
     char *s;

     p = X(mkprinter_cnt)(&cnt);
     plnr->adt->exprt(plnr, p);
     X(printer_destroy)(p);

     s = (char *) non_fftw_malloc(sizeof(char) * (cnt + 1), OTHER);

     p = X(mkprinter_str)(s);
     plnr->adt->exprt(plnr, p);
     X(printer_destroy)(p);

     return s;
}