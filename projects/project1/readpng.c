// Emma Akbari (eea21) 
// Note: I implemented the extra credit in methods add_text and add_chunk
// (./readpng filename.png add Name "value")
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// read the assignment instructions about this!
#define MAX_REASONABLE_TEXT_CHUNK_SIZE 1024

// the PNG file signature.
const char PNG_SIGNATURE[8] = {137, 'P', 'N', 'G', '\r', '\n', 26, '\n'};

// see if two strings are equal.
bool streq(const char* a, const char* b) { return strcmp(a, b) == 0; }

// see if the first n characters of two strings are equal.
bool strneq(const char* a, const char* b, int n) { return strncmp(a, b, n) == 0; }

// given a chunk's identifier and a type, sees if they're equal. use like:
//    <read a chunk's length and identifier>
//    if(is_chunk(identifier, "IHDR"))
bool is_chunk(const char* identifier, const char* type) {
	return strneq(identifier, type, 4);
}

// byte-swaps a 32-bit integer.
unsigned int bswap32(unsigned int x) {
	return
		((x & 0xFF) << 24) |
		((x & 0xFF00) << 8) |
		((x & 0xFF0000) >> 8) |
		((x & 0xFF000000) >> 24);
}

// function returns png file if valid
FILE* open_file(const char* fname) {
	FILE* f = fopen(fname, "rb+");

	if(f == NULL) {
		printf("Error: could not open file.\n");
		exit(1); // exit if file invalid
	}

	char pngSig[8];

	// read PNG file signature into array
	fread(&pngSig, sizeof(pngSig), 1, f);

	bool png = strneq(PNG_SIGNATURE, pngSig, 8);

	if(!png) { // error if not a png
		printf("Error: not a png.\n");
		exit(1);
	}

	return f; 
}

// struct for chunk header (length & identifier)
typedef struct {
	unsigned int length;
	char identifier[4];
} ChunkHeader; 

// read a chunk's header and return as a struct
ChunkHeader read_header(FILE* f) {
	ChunkHeader header;
	fread(&header, sizeof(header), 1, f);
	header.length = bswap32(header.length);
	return header;
}

// struct for chunk data
typedef struct {
	unsigned int width;
	unsigned int height;
	char colorBitDepth;
	char colorType;
	char compression;
	char filtering;
	char interlaced;
} ChunkData;

// read a chunk's data and return as a struct
ChunkData read_data(FILE* f) {
	ChunkData data;
	fread(&data, 13, 1, f);
	data.width = bswap32(data.width);
	data.height = bswap32(data.height);
	return data;
} 

// determine ChunkData colorType and return as string
const char* get_type(char color) {
	const char* type;

	switch(color) {
		case 0: type = "Grayscale";
				break;
		case 2: type = "RGB";
				break;
		case 3: type = "Indexed";
				break;
		case 4: type = "Grayscale + Alpha";
				break;
		case 6: type = "RGB + Alpha";
				break;
		default: type = "Invalid";
	}

	return type;
}

// determine ChunkData interlaced and return yes or no
const char* get_interlaced(char inter) {
	const char* laced;

	switch(inter) {
		case 0: laced = "no";
				break;
		case 1: laced = "yes";
				break;
		default: laced = "Invalid";
	}

	return laced;
}

// ------------------------------------------------------------------------------------------------
// Commands (this is what you'll implement!!!!)
// ------------------------------------------------------------------------------------------------

// open png file and display information
void show_info(const char* filename) {
	FILE* file = open_file(filename);
	read_header(file); // read in chunk header
	ChunkData fileInfo = read_data(file); // read in chunk data
	// display png info
	printf("File info:\n");
	printf("\tDimensions: %d x %d\n", fileInfo.width, fileInfo.height);
	printf("\tBit depth: %d\n", fileInfo.colorBitDepth);
	printf("\tColor type: %s\n", get_type(fileInfo.colorType));
	printf("\tInterlaced: %s\n", get_interlaced(fileInfo.interlaced));

	fclose(file);
}

// open png file and display chunks
void dump_chunks(const char* filename) {
	FILE* file = open_file(filename);
	ChunkHeader chunk;

	// print chunks
	while(true){
		chunk = read_header(file);
		printf("'%.4s' ", chunk.identifier); // print identifier
		printf("(length = %d)\n", chunk.length); // print length
		if(is_chunk(chunk.identifier, "IEND")) {
			break; // end loop at "IEND" chunk
		}
		fseek(file, (chunk.length + 4), SEEK_CUR); // go to next chunk
	}

	fclose(file);
}

// helper method to show_text, read in and print text data
void print_text(FILE* file, ChunkHeader textChunk) {
	// read in name and value (textData)
	char* textData = malloc(textChunk.length + 1);
	fread(textData, textChunk.length, 1, file);
	textData[textChunk.length] = '\0'; //zero terminate the value
	
	// copy value into new array
	char value[textChunk.length - strlen(textData)]; 
	strncpy(value, textData + strlen(textData) + 1, textChunk.length - strlen(textData));

	// print name and value
	printf("%s:\n", textData); // print name
	printf("  %s\n\n", value); // print value
	fseek(file, 4, SEEK_CUR); // go to next chunk
	free(textData); 
}

// open file and display any tEXt chunks
void show_text(const char* filename) {
	FILE* file = open_file(filename);
	ChunkHeader chunk;

	while(true){
		// read the next chunk header
		chunk = read_header(file);
	
		if(is_chunk(chunk.identifier, "IEND")) {
			break; // end loop at "IEND" chunk
		}
		else if(is_chunk(chunk.identifier, "tEXt")) {
			if(chunk.length > MAX_REASONABLE_TEXT_CHUNK_SIZE) {
				// skip large text chunks
				printf("Error: text chunk too big.\n");
				exit(1);
			}
			else {
				// read and print text chunk
				print_text(file, chunk);
			}
		}
		else {
			// skip non text chunks
			fseek(file, (chunk.length + 4), SEEK_CUR);
		}
	} // end while

	fclose(file);
}

// helper method to add_text, adds a text chunk and iend chunk
void add_chunk(FILE* file, const char* name, const char* val) {
	// overwrite header of original IEND
	fseek(file, -8, SEEK_CUR); 
	// set up different segments of text chunk
	unsigned int length = bswap32(strlen(name) + strlen(val) + 1);
	char text[4] = "tEXt";
	char null = '\0';
	char crc[4] = {0, 0, 0, 0};
	// write new text chunk
	fwrite(&length, 4, 1, file);
	fwrite(text, 4, 1, file);
	fwrite(name, strlen(name), 1, file);
	fwrite(&null, 1, 1, file);
	fwrite(val, strlen(val), 1, file);
	fwrite(crc, 4, 1, file);

	// set up variables for IEND chunk
	length = 0;
	char iend[4] = "IEND";
	// write new IEND chunk
	fwrite(&length, 4, 1, file);
	fwrite(iend, 4, 1, file);
	fwrite(crc, 4, 1, file); 
}

// add text chunk to png file
void add_text(const char* filename, const char* textName, const char* textValue) {
	FILE* file = open_file(filename);
	ChunkHeader chunk;

	//check that name is 1 to 79 chars long
	if(strlen(textName) < 1 || strlen(textName) > 79) {
		printf("Error: name length must be between 1 and 79\n");
		exit(1);
	}

	// find IEND chunk and overwrite
	while(true){
		chunk = read_header(file);
		if(is_chunk(chunk.identifier, "IEND")) {
			add_chunk(file, textName, textValue);
			break; 
		}
		fseek(file, (chunk.length + 4), SEEK_CUR); // continue if not IEND chunk
	}

	fclose(file);
}

// ------------------------------------------------------------------------------------------------
// Argument parsing
// ------------------------------------------------------------------------------------------------

typedef enum {
	Info,
	DumpChunks,
	Text,
	Add,
} Mode;

typedef struct {
	const char* input;
	Mode mode;
	const char* name;
	const char* value;
} Arguments;


void show_usage_and_exit(const char* reason) {
	if(reason) {
		printf("Error: %s\n", reason);
	}

	printf("Usage:\n");
	printf("  ./pngedit input.png [command]\n");
	printf("Commands:\n");
	printf("  (no command)         displays PNG file info.\n");
	printf("  dump                 dump all chunks in the file.\n");
	printf("  text                 show all text chunks in the file.\n");
	printf("  add Name \"value\"     add text chunk to end of file.\n");
	exit(1);
}

Arguments parse_arguments(int argc, const char** argv) {
	Arguments ret = {};

	switch(argc) {
		case 1: show_usage_and_exit(NULL);
		case 2: ret.mode = Info; break;
		case 3: {
			if(streq(argv[2], "dump")) {
				ret.mode = DumpChunks;
			} else if(streq(argv[2], "text")) {
				ret.mode = Text;
			} else {
				show_usage_and_exit("Invalid command.\n");
			}
			break;
		}
		case 5: {
			if(streq(argv[2], "add")) {
				ret.mode = Add;
			} else {
				show_usage_and_exit("Invalid command.\n");
			}
			break;
		}
		default: show_usage_and_exit("Invalid arguments.");
	}

	// if we get here, argv[1] is valid.
	ret.input = argv[1];
	ret.name = argv[3];
	ret.value = argv[4];
	return ret;
}

int main(int argc, const char** argv) {
	Arguments args = parse_arguments(argc, argv);

	switch(args.mode) {
		case Info:       show_info(args.input);   break;
		case DumpChunks: dump_chunks(args.input); break;
		case Text:       show_text(args.input);   break;
		case Add:		 add_text(args.input, args.name, args.value); break;
		default:
			printf("well this should never happen!\n");
			return 1;
	}

	return 0;
}