/*
 * Copyright (C) 2014 by Maksym Borodin <Borodin.Maksym@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CHATPAGEWIDGETFACTORY_H
#define CHATPAGEWIDGETFACTORY_H

#include <vector>

#include <stdint.h>

#define REGISTER_CHATWIDGET(factory) int __temp_var_##factory##__ = ChatPageWidgetFactory::instance().registerMethod(new factory())

class IChatPageWidget;
class QWidget;

class ChatPageWidgetFactory
{
public:
    class Method {
    public:
      virtual bool isProviderFor(int64_t id) const = 0;
      virtual IChatPageWidget* construct(int64_t id, QWidget* parent) = 0;
  };
  
private:
  ChatPageWidgetFactory() {}
  std::vector<Method*> methods;

public:
  static ChatPageWidgetFactory& instance() {
      static ChatPageWidgetFactory singleton;
      
      return singleton;
  }
  
  int registerMethod(ChatPageWidgetFactory::Method* method);
  IChatPageWidget* construct(int64_t id, QWidget* parent) const;
};

#endif // CHATPAGEWIDGETFACTORY_H
