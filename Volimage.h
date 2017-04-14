//WLSGRA012
#ifndef VOLIMAGE_H
#define VOLIMAGE_H
#include <iostream>

using namespace std;

namespace WLSGRA012{

	class VolImage
	{
		private:
			//429 303 123
			int width, height, numberOfImages;
			 // data for each slice, in order
			vector<unsigned char**> slices;


		public:
			VolImage();
			VolImage(int width, int height, int numberOfImages);
			 // destructor - define in .cpp file
			~VolImage();


			// populate the object with images in stack and
			//set member variables
			bool readImages(string baseName);

			// compute difference map and write out
			void diffmap(int sliceI, int sliceJ, string output_prefix);

			// extract slice sliceId and write to output
			void extract(int sliceId, string output_prefix);

			// number of bytes uses to store image data bytes
			//and pointers (ignore vector<> container, dims etc)
			int volImageSize(void);

			void getDimensions();
	};

}

#endif
