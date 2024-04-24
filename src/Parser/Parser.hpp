#include <iostream>
#include <string>
#include <vector>
#include <sstream>

struct LightParserType {
    float x, y, z;
    int r, g, b;
};

struct ObjectParserType {
    std::string path;
    std::string texture;
};

struct CharacterParserType {
    float x, y, z;
};

std::vector<LightParserType> lights;
std::vector<ObjectParserType> objects;
std::vector<CharacterParserType> characters;

void parseLight(const std::string& line) {
    std::istringstream iss(line);
    char c;
    LightParserType light;

    if (iss >> c && c == '[' && iss >> light.x && iss >> c && c == ',' &&
        iss >> light.y && iss >> c && c == ',' && iss >> light.z && iss >> c && c == ']') {
        if (iss >> c && c == '[' && iss >> light.r && iss >> c && c == ',' &&
            iss >> light.g && iss >> c && c == ',' && iss >> light.b && iss >> c && c == ']') {
            lights.push_back(light);
        }
    }
}

void parseObject(const std::string& line) {
    std::istringstream iss(line);
    char c;
    ObjectParserType object;

    if (iss >> c && c == '[' && std::getline(iss, object.path, '"') && iss >> c && c == ']') {
        if (iss >> c && c == '[' && std::getline(iss, object.texture, '"') && iss >> c && c == ']') {
            objects.push_back(object);
        }
    }
}

void parseCharacter(const std::string& line) {
    std::istringstream iss(line);
    char c;
    CharacterParserType character;

    if (iss >> c && c == '[' && iss >> character.x && iss >> character.y && iss >> character.z &&
        iss >> c && c == ']') {
        characters.push_back(character);
    }
}

void parseMapData(const std::string& mapData) {
    std::istringstream iss(mapData);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.find("LIGHT") == 0) {
            parseLight(line.substr(6));
        }
        else if (line.find("OBJECT") == 0) {
            parseObject(line.substr(7));
        }
        else if (line.find("CHARACTER") == 0) {
            parseCharacter(line.substr(10));
        }
    }
}

int main() {
    std::string mapData = R"(
        LIGHT([1.0,2.0,3.0][255,0,0])
        OBJECT(["path/to/object"]["texture.png"])
        CHARACTER([4.0,5.0,6.0])
    )";

    parseMapData(mapData);

    // Print the parsed data
    for (const auto& light : lights) {
        std::cout << "Light: [" << light.x << "," << light.y << "," << light.z << "]";
        std::cout << " [" << light.r << "," << light.g << "," << light.b << "]\n";
    }

    for (const auto& object : objects) {
        std::cout << "Object: [\"" << object.path << "\"] [\"" << object.texture << "\"]\n";
    }

    for (const auto& character : characters) {
        std::cout << "Character: [" << character.x << "," << character.y << "," << character.z << "]\n";
    }

    return 0;
}