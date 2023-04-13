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

#ifndef BUTTON_HPP
#  define BUTTON_HPP

#  include <SFML/Graphics.hpp>
#  include <functional>
#  include <string>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

//------------------------------------------------------------------------------
inline static std::string data_path(std::string const& file)
{
    return std::string("/home/qq/MyGitHub/GUI/data/") + file;
}

// *****************************************************************************
//! \brief
// *****************************************************************************
class GenericButton
{
public:

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    GenericButton(std::string const& texture_on_path,
                  std::string const& texture_off_path,
                  sf::Vector2f const& dim)
    {
        m_textures[0].loadFromFile(texture_on_path);
        m_textures[1].loadFromFile(texture_off_path);

        m_rectangle.setSize(dim);
        m_rectangle.setTexture(&m_textures[1]);

        if (!m_font.loadFromFile(data_path("font.ttf")))
        {
            std::cerr << "Could not load font file ..." << std::endl;
            exit(1);
        }

        m_text.setFont(m_font);
        m_text.setCharacterSize(24);
        m_text.setFillColor(sf::Color::Black);
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    //template<typename Functor>
    //void setCallbacks(Functor fon, Functor foff)
    void setCallbacks(std::function<void()> fon, std::function<void()> foff)
    {
        m_callbacks[0] = [=]() { fon(); };
        m_callbacks[1] = [=]() { foff(); };
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    inline void setStatus(const bool on_state = true)
    {
        m_rectangle.setTexture(&m_textures[!on_state]);
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    inline void setPosition(sf::Vector2f const& pos)
    {
        m_rectangle.setPosition(pos);
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    bool click(sf::Vector2f const& mouse)
    {
        if (m_rectangle.getGlobalBounds().contains(mouse.x, mouse.y))
        {
            click();
            return true;
        }
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    void click()
    {
        if (m_rectangle.getTexture() == &m_textures[0])
        {
            m_rectangle.setTexture(&m_textures[1]);
            if (m_callbacks[1] != nullptr)
            {
                m_callbacks[1]();
            }
        }
        else
        {
            m_rectangle.setTexture(&m_textures[0]);
            if (m_callbacks[0] != nullptr)
            {
                m_callbacks[0]();
            }
        }
    }


    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    void draw(sf::RenderWindow& renderer)
    {
        renderer.draw(m_rectangle);

        m_text.setString(caption);
        float x = m_rectangle.getPosition().x + m_rectangle.getSize().x / 2.0f;
        float y = m_rectangle.getPosition().y + m_rectangle.getSize().y;

        m_text.setPosition(x - m_text.getLocalBounds().width / 2.0f,
                           y - m_text.getLocalBounds().height +
                           float(m_text.getCharacterSize()));
        renderer.draw(m_text);
    }

public:

    std::string caption;

private:

    sf::Texture m_textures[2];
    sf::RectangleShape m_rectangle;
    sf::Font m_font;
    sf::Text m_text;
    std::function<void()> m_callbacks[2] = {nullptr, nullptr};
};

#endif
