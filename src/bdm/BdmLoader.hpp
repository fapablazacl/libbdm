

#pragma once

#ifndef __bdmloader__
#define __bdmloader__

#include <cassert>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>

namespace bdm {
    // New line character
    static const int Separator = 0x0A;

    struct Header {
        std::uint16_t textureCount;
        std::uint16_t vertexCount;
        std::uint16_t unknown03;
        std::uint16_t faceCount;
        std::uint16_t unknown05;

        friend std::ostream& operator<< (std::ostream& os, const Header &header) {
            os << "Texture Count: " << header.textureCount << std::endl;
            os << "Vertex Count: " << header.vertexCount << std::endl;
            os << "Unknown03: " << header.unknown03 << std::endl;
            os << "Face Count (?): " << header.faceCount << std::endl;
            os << "Unknown05: " << header.unknown05 << std::endl;

            return os;
        }
    };
    
    template<typename Field>
    Field read(std::istream &fs) {
        if (fs.eof()) {
            throw std::runtime_error("EOF reached.");
        }

        Field field = {};
    
        if (fs.eof()) {
            return field;
        }

        fs.read(reinterpret_cast<char*>(&field), sizeof(Field));
    
        return field;
    }

    template<typename Field>
    std::vector<Field> read(std::istream &fs, int count) {
        assert(fs.tellg() >= 0);

        std::vector<Field> fields;

        while ( (--count >= 0) && !fs.eof() ) {
            fields.push_back(read<Field>(fs));
        }

        return fields;
    }

    inline std::vector<std::string> readTextures(std::istream &fs, const Header &header) {
        std::vector<std::string> textures;
    
        // output texture filenames
        for (int i=0; i<header.textureCount; ++i) {
            bool done = false;
            std::string textureName = "";

            while (!done) {
                int character = fs.get();

                if (character == bdm::Separator) {
                    done = true;
                } else {
                    textureName += static_cast<char>(character);
                }
            }

            textures.push_back(textureName);
        }

        return textures;
    }

    template<typename Type, int Count>
    struct Array {
        Type values[Count] = {};

        friend std::ostream& operator<< (std::ostream &os, const Array<Type, Count> &v) {
            for (int i=0; i<Count - 1; i++) {
                os << v.values[i] << ", ";
            }

            os << v.values[Count - 1];

            return os;
        }
    };

    typedef Array<float, 4>            Color;
    typedef Array<float, 3>            Vertex;
    typedef Array<std::uint16_t, 3>    Face;
    typedef Array<float, 2>            TexCoord;
}

#endif
