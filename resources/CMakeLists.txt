juce_add_binary_data(BinaryData SOURCES
    logo.png
    Prince.ico
    Prince.png

    ../models/od.json
    ../models/boost.json
    ../models/dist.json

    gain.png
    vol.png
    tone.png
    sw1.png
    sw2.png
    sw3.png
    background.png

)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
