/*
** Copyright (C) 2009-2014 Quadrant Information Security <quadrantsec.com>
** Copyright (C) 2009-2014 Champ Clark III <cclark@quadrantsec.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* sagan-meta-content.c - This allows content style "searching" that 
 * involve variables.  For example,  if we wanted to search for "bob", 
 * "frank" and "mary",  we'd typically need three content rules.  
 * This allows one rule with the $USER variable for "bob", "frank" and
 * "mary".  
 *
 * meta_content: "Username: ", $USERNAME"; meta_nocase; 
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"             /* From autoconf */
#endif

#include <stdio.h>
#include <string.h>
#include "sagan.h"
#include "sagan-defs.h"

struct _Rule_Struct *rulestruct;

int Sagan_Meta_Content_Search(char *syslog_msg, int rule_position )
{

char *ptmp = NULL;
char *tok = NULL;
char tmp[1024] = { 0 };
char tmp_search[512] = { 0 }; 	
int results = 0;
int match = 0; 
int return_code = 0;
int z;

if ( rulestruct[rule_position].meta_content_not[z] == 0 ) { 

	/* Normal "meta_content" (ie - not !) search */

	for(z=0; z<rulestruct[rule_position].meta_content_count; z++)
		{

		strlcpy(tmp, rulestruct[rule_position].meta_content[z], sizeof(tmp));
		ptmp = strtok_r(tmp, ",", &tok);

			while (ptmp != NULL )
				{

				/* Search for "content help" + "content" */

				snprintf(tmp_search, sizeof(tmp_search), "%s%s", rulestruct[rule_position].meta_content_help[z], ptmp);

				if ( rulestruct[rule_position].meta_content_case[z] == 1 )
					{
					if (strcasestr(syslog_msg, tmp_search)) results++;
                        		}
                    				else
		                        {
                            		if (strstr(syslog_msg, tmp_search))  results++;
                        		}
                    
				ptmp = strtok_r(NULL, ",", &tok);

                		} // End of while(ptmp)
		} // End of for(z) loop

} else { 

	/* Content! search */
	
	for(z=0; z<rulestruct[rule_position].meta_content_count; z++)
		{

		strlcpy(tmp, rulestruct[rule_position].meta_content[z], sizeof(tmp));
		ptmp = strtok_r(tmp, ",", &tok);

			while (ptmp != NULL )
				{

				snprintf(tmp_search, sizeof(tmp_search), "%s%s", rulestruct[rule_position].meta_content_help[z], ptmp);

                        	if ( rulestruct[rule_position].meta_content_case[z] == 1 )
					{
					if (strcasestr(syslog_msg, tmp_search)) match++;
					}
						else
					{
					if (strstr(syslog_msg, tmp_search)) match++; 
					}

					ptmp = strtok_r(NULL, ",", &tok);

                		} // End of while(ptmp)
		} // End of for(z) loop


	/* content! we do NOT want "match".  Zero means nothing matches! */
	if ( match == 0 ) results++; 
	
} // end/else of if  meta_content_not ...

if ( results == rulestruct[rule_position].meta_content_count) return(TRUE);	

return(FALSE);

}

