# TextFileInput
this is c++ class(-es) providing Text Input from UTF-8 Encoded files ( 7-bit ASCII as well).
- - -
## Why not standard std::ifstream ?
although a lot more powerful, but it does not keep trace of position in file expressed in lines and columns. Primary objective of these classes is to keep track of it.

- - -

## When to use this class ?
In cases where you intend to parse text file and report information about errors , for example reading configuration from file

```
[Theme]
Name= ĀžukalnaDubļi
BackgroundColor=#794c13


```

While parsing you encounter symbol "Ā" , but you will accept only ASCII characters, so you would like to create message like 
`Invalid character 'Ā' in 'settings.ini' Line 2 Column 7. Only alphanumeric ASCII characters are allowed`

Normally you would have to track your location in file. This class does the tracking for you.

In addition to keeping track on row(line) and column information of red characters, this class also provides ability to seek to specific character in file providing it's row(line) and column numbers. 

Note : indexing is 1-based as text editors usually have 1-based indexing
- - -
## What do we consider as line end characters ?	
often we are accustomed to LF or CRLF being used as line end characters , however according to Wikipedia , Unicode standard defines a number of characters that applications should recognize as line terminators:
- LF ( U+000A )
- VT ( U+000B )
- FF ( U+000C )
- CR ( U+000D )
- CR+LF ( U+000D 0+000A )
- NEL ( U+0085 )
- LS ( U+2028 )
- PS ( U+2029 )
* * * *

### Classes and their purposes:

- **TextFileInput_decoder_base** - provides base class for character decoders, thus in future allowing to parse different encodings , not only UTF-8. **Do not direclty use this class**
- **TextFileInput_decoder_utf8** - provides decoding from UTF-8 to unicode codepoints

### Branches: 

**master** - contains only header files needed, only "tested versions"

**develop** - contains header files and CodeBlocks project for testing purposes

**encodings** - contains development of encoders