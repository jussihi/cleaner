/*
 * Function definitions of the cleanerlib library
 */ 


/*
 * Function indent_code
 * @param filename - The filename of the original code file
 * @param pad - The padding user wants to apply, for example "  " (two spaces)
 * @reuturn void - returns nothing
 */
void indent_code(const char* filename, const char *pad);

/*
 * Function remove_comments
 * @param filename - The filename of the original code file
 * @return int - 	 The amount of comments removed. This function 
 * 					 returns a negative integer if an error occured.
 */
int remove_comments(const char* filename);