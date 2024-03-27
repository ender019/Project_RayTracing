#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode(1024, 768), "Hello, World!", sf::Style::Close);  // Создать окно
  // Ограничить частоту кадров в секунду до 60
  window.setFramerateLimit(60);
  // Основной цикл
  while (window.isOpen()) {
    sf::Event event;  // События
    // Обработка событий (нажатие кнопок, закрытие окна и т.д.)
    while (window.pollEvent(event)) {
      // Закрыть окно если нажата кнопка "Закрыть"
      if (event.type == sf::Event::Closed) window.close();
    }
    window.clear(sf::Color::Black);  // Очистить окно и залить его черным цветом
    window.display();  // Отобразить
  }
}