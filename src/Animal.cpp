#include "Animal.h"
#include <iostream>

extern sf::Font font;
extern sf::FloatRect screenDimension;

Animal::Animal(std::string path) : sprite_(path), text_("",font,20){
	sprite_.setFrameRate(sf::milliseconds(200));
	userTick_ = sf::milliseconds(500);
	text_.setColor(sf::Color::Blue);
	isSayingSomething_ = false;
	userTickCounter_ = sf::Time::Zero;
}

void Animal::setPosition(sf::Vector2f position){
	sprite_.setPosition(position);
	text_.setPosition({position.x-30, position.y-20});
}

void Animal::setAnimation(Direction animationName){
	sprite_.setAnimation(animationName);
}

void Animal::move(Direction direction){
	sf::Vector2f position = sprite_.getPosition();
	switch(direction){
		case Direction::Left:
			position.x--;
			break;
		case Direction::Up:
			position.y--;
			break;
		case Direction::Right:
			position.x++;
			break;
		case Direction::Down:
			position.y++;
			break;
		default: break;
	}
	
	sf::IntRect currentFrame = sprite_.getCurrentFrame();
	sf::Vector2f topLeftPointOfFrame(position);
	sf::Vector2f bottomRightPointOfFrame(position + sf::Vector2f(currentFrame.width, currentFrame.height));
	
	if(screenDimension.contains(topLeftPointOfFrame) and screenDimension.contains(bottomRightPointOfFrame)){
		sprite_.setPosition(position);
	}
}

void Animal::operator<<(std::string text){
	text_.setString(text);
	isSayingSomething_ = true;
	frameCountThatSomethingIsBeingSaid_ = 0;
}

void Animal::update(sf::Time elapsedTime){
	sprite_.update(elapsedTime);
	
	if(isSayingSomething_){
		frameCountThatSomethingIsBeingSaid_++;
	}
	
	if(frameCountThatSomethingIsBeingSaid_ > 30){
		text_.setString("");
		isSayingSomething_ = false;
		frameCountThatSomethingIsBeingSaid_ = 0;
	}
	
	userTickCounter_ += elapsedTime;
	if(userTickCounter_ > userTick_){
		userFunction();
		userTickCounter_ -= userTick_;
	}
}

void Animal::drawTextBubble(sf::RenderTarget& target, sf::RenderStates states) const{
	//calculate position of text bubble
	float textWidth = text_.getGlobalBounds().width;
	float textHeight = text_.getGlobalBounds().height;
	sf::CircleShape textBubble(textWidth/2);
	textBubble.setOrigin(textWidth/2, textWidth/2);
	sf::Vector2f textBubblePosition = text_.getPosition();
	textBubblePosition.x += textWidth/2;
	textBubblePosition.y += 0.9*textHeight; // have a better rendering
	textBubble.setPosition(textBubblePosition);
	
	//kikoo bubble
	textBubble.setFillColor(sf::Color(225,200,250));
	textBubble.setOutlineThickness(1.5);
	textBubble.setOutlineColor(sf::Color(125,100,150));
	textBubble.setScale(1.6,1.6*textHeight/textWidth);
	
	target.draw(textBubble,states);
	target.draw(text_,states);
}

void Animal::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(sprite_, states);
	if(isSayingSomething_){
		drawTextBubble(target, states);
	}
}

void Animal::setUserTick(sf::Time userTick){
	userTick_ = userTick;
}

void Animal::userFunction(){
}

