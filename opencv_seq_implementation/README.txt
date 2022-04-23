/*
 * File name    : README.txt
 * Authors      : Avinash Eega (aeega) 
 * Created on   : Mar 25, 2022
 * Credits      : Yasin Yıldırım @ Simit Lab
 * Note         : Majority of the codebase in this file is taken from Yasin Yildirim's git hub. 
 * orignal code : https://github.com/yasinyildirim/CannyEdgeDetector 
 * Updates      : 1. The original code base is implemented with 5x5 kernels in every stage. 
 *                      We have updated the code base to support a 3x3 kernel implementation. 
 *                2. Included rdtsc() functions to measure time taken for each stage to execute
 */




Run command:
    // To run CED algorithm on the image
    make ced image=<image_name>
   
Example commands

    // To run CED algorithm on the flower.jpeg
    make ced image=flower.jpeg
    
    // To run CED algorithm on the alba_flores.jpeg
    make ced image=alba_flores.jpeg
