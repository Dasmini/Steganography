# 🖼️ Steganography using LSB Encoding in C

A command-line application written in C that hides and retrieves secret text messages inside BMP images using the Least Significant Bit (LSB) steganography technique.

---

## 📖 About

Steganography is the practice of concealing information inside another file so that the existence of the hidden data is not obvious.

This project encodes a text file into a BMP image by modifying the Least Significant Bit (LSB) of the image bytes. Since only one bit of each byte is altered, the visual appearance of the image remains virtually unchanged.

---

## ✨ Features

- Encode secret text into a BMP image
- Decode the hidden message from the encoded image
- Command-line interface
- Validates file extensions
- Preserves image quality using LSB encoding
- Handles secret files of varying sizes
- Modular implementation using multiple source files

---

## 🛠️ Technologies Used

- C Programming
- GCC
- Git

---

## 📂 Project Structure

```
.
├── Screenshots/
|       ├── Decode output.png
|       ├── Encode output.png
|       ├── Image with encoded data.png
|       └── Source image.png
├── encode.c
├── encode.h
├── decode.c 
├── decode.h 
├── common.h
├── types.h
├── test_encode.c
├── README.md
├── secret.txt
├── output.txt
├── stego.bmp
└── beautiful.bmp
```

---

## ⚙️ How LSB Encoding Works

Each character consists of **8 bits**.

For every character:

- Read 8 bytes from the image
- Replace the Least Significant Bit of each image byte with one bit of the character
- Write the modified bytes into the output image

Example:

```
Character : A

Binary:
01000001

Image Bytes:

10010110
11101011
01001000
10101110
11110001
00011110
11000110
01101001

↓

Replace only the last bit of every byte.

Modified Image Bytes:

10010110
11101011
01001000
10101110
11110000
00011110
11000110
01101001
```

Only the last bit changes, making the modification imperceptible to the human eye.

---

## 🚀 Build

Compile the project using:

```bash
gcc *.c
```

---

## ▶️ Usage

### Encode

```bash
./a.out -e beautiful.bmp secret.txt stego.bmp
```

If the output image name is omitted:

```bash
./a.out -e beautiful.bmp secret.txt
```

The program automatically creates:

```
stego.bmp
```

---

### Decode

```bash
./a.out -d stego.bmp output.txt
```
If the output file name is omitted:
```bash
./a.out -d stego.bmp
```
The program automatically creates:

```
output.txt
```

---

## 📌 Encoding Flow

```
Read Command Line Arguments
          │
          ▼
Validate Files
          │
          ▼
Open Files
          │
          ▼
Copy BMP Header
          │
          ▼
Encode Magic String
          │
          ▼
Encode Secret File Extension
          │
          ▼
Encode Secret File Size
          │
          ▼
Encode Secret Data
          │
          ▼
Copy Remaining Image Data
          │
          ▼
Generate Stego Image
```

---

## 📌 Decoding Flow

```
Read Command Line Arguments
          │
          ▼
Open Stego Image
          │
          ▼
Skip BMP Header
          │
          ▼
Decode Magic String
          │
          ▼
Decode File Extension
          │
          ▼
Decode File Size
          │
          ▼
Decode Secret Data
          │
          ▼
Create Output File
```

---

## 📚 Concepts Covered

- Bit Manipulation
- LSB Encoding
- Binary File Handling
- BMP File Format
- Command Line Arguments
- Modular Programming
- Error Handling
- File Pointers
- Structures
- Enumerations

---

## 📸 Sample

### Encoding output
![Encoding output](/Screenshots/Encode%20output.png)

### Decoding output
![Decoding output](/Screenshots/Decode%20output.png)

### Original image before encoding
![Original image before encoding](/Screenshots/Source%20image.png)

### Image with data encoded in it
![Image with data encoded in it](/Screenshots/Image%20with%20Encoded%20data.png)

---

## 🎯 Learning Outcomes

Through this project, I gained hands-on experience with:

- Low-level bit manipulation
- Binary file processing
- Working with BMP image format
- Modular software design
- Command-line utilities in C
- Debugging file handling issues
- Practical implementation of steganography concepts

---

## 👩‍💻 Author

**Dasmini D ❤️**

Hey check out [my GitHub](https://github.com/Dasmini) and [my LinkedIn](https://www.linkedin.com/in/dasmini-d-265b3920b/)

---