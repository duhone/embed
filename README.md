# embed
Utility to embed binary data into an executable. Given an input file, it will create a header/source pair with the input file encoded into the source file as binary data in hex. The header Will provide that data as a CR::Core::Span<std::byte>.
