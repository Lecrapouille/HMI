//=====================================================================
// TimedPetriNetEditor: A timed Petri net editor.
// Copyright 2021 -- 2022 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of TimedPetriNetEditor.
//
// TimedPetriNetEditor is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Application.hpp"
#include "MQTT.hpp"
#include "Button.hpp"
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

//------------------------------------------------------------------------------
static void publish(MQTT& client, bool status, std::string const& caption)
{
    std::string message("0");

    message[0] = '0' + status;
    std::cout << caption << ": " << (status ? "ON" : "OFF") << std::endl;
    client.publish("GEMMA/" + caption, message, 0);
}

// *****************************************************************************
//! \brief
// *****************************************************************************
class PowerSwitch: public GenericButton
{
public:

    PowerSwitch(MQTT& client)
        : GenericButton(data_path("marche.png"),
                        data_path("arret.png"),
                        sf::Vector2f(78.f, 142.f)),
          m_client(client)
    {
        setCallbacks([&](){ on(); }, [&](){ off(); });
    }

private:

    void on() { publish(m_client, true, caption); }
    void off() { publish(m_client, false, caption); }

private:

    MQTT& m_client;
};

// *****************************************************************************
//! \brief
// *****************************************************************************
class ToggleSwitch: public GenericButton
{
public:

    ToggleSwitch(MQTT& client)
        : GenericButton(data_path("toggleup.png"),
                        data_path("toggledown.png"),
                        sf::Vector2f(64.f, 70.f)),
          m_client(client)
    {
        setCallbacks([&](){ on(); }, [&](){ off(); });
    }

private:

    void on() { publish(m_client, true, caption); }
    void off() { publish(m_client, false, caption); }

private:

    MQTT& m_client;
};

// *****************************************************************************
//! \brief
// *****************************************************************************
class ButtonSwitch: public GenericButton
{
public:

    ButtonSwitch(MQTT& client)
        : GenericButton(data_path("toggleup.png"),//bt_vert_up.png"),
                        data_path("toggledown.png"),//"bt_vert_dn.png"),
                        sf::Vector2f(64.f, 64.f)),
          m_client(client)
    {
        setCallbacks([&](){ on(); }, [&](){ off(); });
    }

private:

    void on() { publish(m_client, true, caption); }
    void off() { publish(m_client, false, caption); }

private:

    MQTT& m_client;
};

// *****************************************************************************
//! \brief
// *****************************************************************************
class MyGUI: public Application::GUI, public MQTT
{
public:

    MyGUI(Application& application);

private:

    template<typename T>
    void createButton(std::string const& name, float const x, float const y)
    {
        m_buttons.emplace_back(std::make_unique<T>(*this));
        auto& b = m_buttons.back();
        b->caption = name;
        b->setPosition(sf::Vector2f(x, y));
    }

private: // Derived from Application::GUI

    virtual void onDraw() override;
    virtual void onUpdate(const float dt) override;
    virtual void onHandleInput() override;

private: // MQTT

    //-------------------------------------------------------------------------
    //! \brief Callback when this class is connected to the MQTT broker.
    //-------------------------------------------------------------------------
    virtual void onConnected(int /*rc*/) override
    {}

    //-------------------------------------------------------------------------
    //! \brief Callback when this class is has received a new message from the
    //! MQTT broker.
    //-------------------------------------------------------------------------
    virtual void onMessageReceived(const struct mosquitto_message& /*message*/) override
    {}

private:

    sf::Vector2f m_mouse;
    std::vector<std::unique_ptr<GenericButton>> m_buttons;
};

//------------------------------------------------------------------------------
MyGUI::MyGUI(Application& application)
    : Application::GUI(application, "GUI", sf::Color::White)
{
    connect("localhost", 1883);

    createButton<PowerSwitch>("AU", 100.0f, 100.0f);
    createButton<ButtonSwitch>("Dcy", 200.0f, 100.0f);
    createButton<ButtonSwitch>("Acy", 200.0f, 200.0f);
    createButton<ButtonSwitch>("Rearm", 300.0f, 100.0f);
    createButton<ButtonSwitch>("Valid", 300.0f, 200.0f);
    createButton<ButtonSwitch>("Manu", 400.0f, 150.0f);
    createButton<ButtonSwitch>("Avance", 500.0f, 100.0f);
    createButton<ButtonSwitch>("Recule", 500.0f, 200.0f);
    createButton<ToggleSwitch>("Temp", 600.0f, 150.0f);
}


//------------------------------------------------------------------------------
void MyGUI::onDraw()
{
    for (auto& it: m_buttons)
        it->draw(m_renderer);
}

//------------------------------------------------------------------------------
void MyGUI::onUpdate(float const /*dt*/)
{
}

//------------------------------------------------------------------------------
void MyGUI::onHandleInput()
{
    sf::Event event;
    m_mouse = m_renderer.mapPixelToCoords(sf::Mouse::getPosition(m_renderer));

    while (m_renderer.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            close();
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                for (auto& it: m_buttons)
                {
                    if (it->click(m_mouse))
                    {
                        break;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

//------------------------------------------------------------------------------
int main(/*int argc, char* argv[]*/)
{
    Application application(800, 600, "GUI");
    MyGUI gui(application);
    application.loop(gui);

    return EXIT_SUCCESS;
}
