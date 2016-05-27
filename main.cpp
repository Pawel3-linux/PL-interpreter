else if (types[ind] == t_char)
					{
						char * tocat = (char *) malloc(1000);
						sprintf(tocat, "%c", charv[where[ind]]);
						strcat(sysstr, tocat);
						strcat(sysstr, " ");
						free(tocat);
					}
