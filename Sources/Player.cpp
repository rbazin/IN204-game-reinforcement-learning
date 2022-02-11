#include "./headers/Player.h"

// Init functions
void Player::initVariables()
{
    this->moveSpeed = 1.f; // nombre de pixels parcoururs par appui sur une touche
    this->moveDiag = 1.f / sqrt(2.f);
    this->attackCoolDownMax = 10.f;
    this->attackCoolDown = this->attackCoolDownMax;
}

void Player::initTexture()
{
    // Load texture from file
    if (!this->texture.loadFromFile("../../Textures/white.png"))
    {
        std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file" << std::endl;
    }
    // Same for sword

    if (!this->swordTexture.loadFromFile("../../Textures/sword.png"))
    {
        std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file" << std::endl;
    }
}

void Player::initSprite()
{
    // Set texture to sprite
    this->sprite.setTexture(this->texture);
    this->swordSprite.setTexture(this->swordTexture);

    // Resize the player sprite and sword sprite
    this->sprite.scale(0.2f, 0.2f);
    this->swordSprite.scale(0.2f, 0.2f);
    this->swordSprite.setOrigin(this->swordSprite.getLocalBounds().width / 2, 0);
}

void Player::initPosition()
{
    this->sprite.setPosition(this->window->getSize().x / 2, this->window->getSize().y / 2);
    this->swordSprite.setPosition(
        this->getPosition().x + this->getBounds().width / 2,
        this->getPosition().y + this->getBounds().height / 2);
}

// Constructors and destructors
Player::Player()
{
    this->initVariables();
    this->initTexture();
    this->initSprite();
    this->initPosition();
}

Player::Player(RenderWindow *window)
{
    this->window = window;
    this->initVariables();
    this->initTexture();
    this->initSprite();
    this->initPosition();
}

Player::~Player()
{
    // Delete fireballs
    for (auto *fireball : this->fireballs)
        delete fireball;
}

// Functions
void Player::renderPlayer(RenderTarget &target)
{
    // target.draw(this->swordSprite);
    target.draw(this->sprite);
}

void Player::renderFireballs(RenderTarget &target)
{

    for (auto *fireball : this->fireballs)
    {
        fireball->renderFireball(target);
    }
}

void Player::move(const float dirX, const float dirY)
{
    this->sprite.move(this->moveSpeed * dirX, this->moveSpeed * dirY);
}

void Player::shootFireballs()
{
    Vector2f direction = this->getMouseDirection();
    Vector2f swordPosition = this->swordSprite.getPosition();
    this->fireballs.push_back(new Fireball(
        swordPosition.x,
        swordPosition.y,
        direction.x,
        direction.y));
}

void Player::updateFireballs()
{
    /*
        Déplacement des fireballs et destruction lorsqu'elles atteignent
        le bord de la fenêtre
    */
    unsigned counter = 0;
    for (auto *fireball : this->fireballs)
    {
        fireball->updateFireball(); // Mouvement fireball

        if ((fireball->getBounds().top + fireball->getBounds().height < 0.f) ||
            (fireball->getBounds().top > this->window->getSize().y) ||
            (fireball->getBounds().left > this->window->getSize().x) ||
            (fireball->getBounds().left + fireball->getBounds().width < 0))
        {
            // Delete fireball if out of bounds
            delete this->fireballs.at(counter);
            this->fireballs.erase(this->fireballs.begin() + counter);
            --counter;
        }

        ++counter;
    }
}

void Player::updateSwordPosition()
{
    this->swordSprite.setPosition(this->getBounds().left + this->getBounds().width / 2, this->getBounds().top + this->getBounds().height / 2);
}

void Player::updateAttack()
{
    if (this->attackCoolDown < this->attackCoolDownMax)
        this->attackCoolDown += 1.f;
}

void Player::updateSwordRotation()
{
    this->swordSprite.setRotation(this->getRotationAngle(this->getMouseDirection().x, this->getMouseDirection().y));
}

void Player::update()
{
    // Update Position
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        this->move(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        this->move(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        this->move(0.f, 1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        this->move(1.f, 0.f);

    // Update tire de fireball
    /*
    if (Mouse::isButtonPressed(Mouse::Left) && this->canAttack())
    {
        this->shootFireballs();
    }
    // Update attack cooldown
    this->updateAttack();

    // Update fireballs position
    this->updateFireballs();

    // Update sword position
    this->updateSwordPosition();

    // Update sword rotation
    this->updateSwordRotation();
    */
}

void Player::update(int action)
{
    // Update Position
    if (action == 0) // haut
        this->move(0.f, -1.f);
    if (action == 1) // gauche
        this->move(-1.f, 0.f);
    if (action == 2) // bas
        this->move(0.f, 1.f);
    if (action == 3) // droite
        this->move(1.f, 0.f);
    if (action == 4) //
        this->move(0.f, 0.f);
    if (action == 5)
        this->move(this->moveDiag, this->moveDiag);
    if (action == 6)
        this->move(-this->moveDiag, this->moveDiag);
    if (action == 7)
        this->move(this->moveDiag, -this->moveDiag);
    if (action == 8)
        this->move(-this->moveDiag, -this->moveDiag);
}

const bool Player::canAttack()
{
    if (this->attackCoolDown >= attackCoolDownMax)
    {
        this->attackCoolDown = 0.f;
        return true;
    }
    return false;
}

// Setters
void Player::setPosition(sf::Vector2f position)
{
    this->sprite.setPosition(position);
}

void Player::setPosition(float x, float y)
{
    this->sprite.setPosition(x, y);
}

// Getters
std::vector<Fireball *> Player::getFireballs()
{
    return this->fireballs;
}

Vector2f Player::getPosition()
{
    return this->sprite.getPosition();
}

Vector2f Player::getMouseDirection()
{
    Vector2f mouseDirection;
    float mouseX = Mouse::getPosition(*this->window).x;
    float mouseY = Mouse::getPosition(*this->window).y;
    float posXCenter = this->getPosition().x;
    float posYCenter = this->getPosition().y;
    float norm = sqrt((posXCenter - mouseX) * (posXCenter - mouseX) + (posYCenter - mouseY) * (posYCenter - mouseY));

    mouseDirection.x = -(posXCenter - mouseX) / norm;
    mouseDirection.y = -(posYCenter - mouseY) / norm;
    return mouseDirection;
}

float Player::getRotationAngle(float X, float Y)
{
    if (Y > 0)
        return std::acos(X) * 180.0f / 3.1415 - 90.f;
    else
        return std::acos(-X) * 180.0f / 3.1415 + 90.f;
}

const FloatRect Player::getBounds() const
{
    return this->sprite.getGlobalBounds();
}