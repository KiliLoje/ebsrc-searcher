# ebsrc scanner

A small script to browse ebsrc-to-listing.   

---

### Compiling

This *should* compile :   

    mkdir -p build
    cd build
    cmake ..
    make

---

### Usage

Make a .txt file with the text that you want to search for.   
Like so :   

    SBC #$50
    AND #$007F

Then execute the binary :   
`./ebsrc-searcher /path/to/ebsrc-to-listing /path/to/target-text.txt`   
You will get an output similar to this :   

    >>> unknown/EF/EF01D2.asm
      EF01F5: E9 50 00        SBC #$50
      EF01F8: 29 7F 00        AND #$007F
    >>> unknown/C4/C43E31.asm
      C43E6C: E9 50 00        SBC #$50
      C43E6F: 29 7F 00        AND #$007F
    >>> unknown/C4/C440B5.asm
      C440E0: E9 50 00        SBC #$50
      C440E3: 29 7F 00        AND #$007F
    >>> unknown/C4/C4424A.asm
      C44272: E9 50 00        SBC #$50
      C44275: 29 7F 00        AND #$007F
    >>> unknown/C4/C445E1.asm
      C44752: E9 50 00        SBC #$50
      C44755: 29 7F 00        AND #$007F
    >>> unknown/C4/C44E61.asm
      C44EEC: E9 50 00        SBC #$50
      C44EEF: 29 7F 00        AND #$007F
    >>> unknown/C4/C44FF3.asm
      C4501D: E9 50 00        SBC #$50
      C45020: 29 7F 00        AND #$007F
    >>> unknown/C4/C4827B.asm
      C48289: E9 50 00        SBC #$50
      C4828C: 29 7F 00        AND #$007F
    >>> unknown/C4/C4999B.asm
      C499A6: E9 50 00        SBC #$50
      C499A9: 29 7F 00        AND #$007F
    >>> ending/render_cast_name_text.asm
      C4E5E6: E9 50 00        SBC #$50
      C4E5E9: 29 7F 00        AND #$007F
    Successfully written links to ebsrc_link.txt

You'd have spotted that last line `Successfully written links to ebsrc_link.txt`   
The program generates links to the specific portion of code spotted inside ebsrc.   

ebsrc_text.txt will look similar to this :   

    https://github.com/Herringway/ebsrc/blob/main/src/unknown/EF/EF01D2.asm#L19-L20
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C43E31.asm#L38-L39
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C440B5.asm#L25-L26
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C4424A.asm#L22-L23
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C445E1.asm#L152-L153
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C44E61.asm#L72-L73
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C44FF3.asm#L27-L28
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C4827B.asm#L16-L17
    https://github.com/Herringway/ebsrc/blob/main/src/unknown/C4/C4999B.asm#L9-L10
    https://github.com/Herringway/ebsrc/blob/main/src/ending/render_cast_name_text.asm#L51-L52
