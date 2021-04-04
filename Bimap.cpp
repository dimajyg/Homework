#include <stdexcept>
#include <optional>
#include <map>
#include <iostream>
#include <vector>
#include <memory>
template <typename Key1, typename Key2, typename Value>
class BiMap {
private:
    std::map<Key2, std::shared_ptr<Value>> key2_to_value;
    std::map<Key1, std::shared_ptr<Value>> key1_to_value;

public:
    // Вставить значение, указав один или оба ключа.
    // Генерирует исключение std::invalid_argument("some text") в случае,
    // если оба ключа пусты, либо один из ключей уже имеется в хранилище.
    void Insert(const std::optional<Key1>& key1 , const std::optional<Key2>& key2,
    const Value& value) {
        if (key1.has_value()) {
            if (key2.has_value()) {
                if (!(key1_to_value.contains(key1.value())) &&
                !(key2_to_value.contains(key2.value()))) {
                    key1_to_value[key1.value()] = std::make_shared<Value>(value);
                    key2_to_value[key2.value()] = key1_to_value[key1.value()];
                } else {
                    throw std::invalid_argument("aaa");
                }

            } else {
                if (!(key1_to_value.contains(key1.value()))) {
                    key1_to_value[key1.value()] = std::make_shared<Value>(value);
                } else {
                    throw std::invalid_argument("aaa");
                }
            }
        } else if (key2.has_value()) {
            if (!(key2_to_value.contains(key2.value()))) {
                key2_to_value[key2.value()] = std::make_shared<Value>(value);
            } else {
                throw std::invalid_argument("aaa");
            }
        } else {
            throw std::invalid_argument("aaa");
        }
    }
    // Получить значение по ключу первого типа.
    // Генерирует исключение std::out_of_range("some text")
    // в случае отсутствия ключа (как и функция at в std::map).
    Value& GetByPrimaryKey(const Key1& key) {
        return *key1_to_value.at(key);
    }
    const Value& GetByPrimaryKey(const Key1& key) const {
        return *key1_to_value.at(key);
    }

    // Аналогичная функция для ключа второго типа.
    Value& GetBySecondaryKey(const Key2& key) {
        return *key2_to_value.at(key);
    }
    const Value& GetBySecondaryKey(const Key2& key) const {
        return *key2_to_value.at(key);
    }
};