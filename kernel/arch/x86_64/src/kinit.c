/*
 * This part of the program initialises the arch specific stuff...
 */

// FIXME: This is ugly. Fix in the future!
extern void kmain();

/**
 * kinit is called by the bootstrap. 
 */
void kinit()
{
	/*
	 * All x86 specific drivers have been initialized. 
	 * Now call the generic main!
	 */
	kmain();
}


