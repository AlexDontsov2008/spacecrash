#include <Book/SceneNode.hpp>
#include <Book/Command.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <algorithm>
#include <cassert>


SceneNode::SceneNode()
: mChildren()
, mParent(nullptr)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&] (Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue& commands)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for(Ptr& child : mChildren)
		child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);

	//drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Ptr& child : mChildren)
		child->draw(target, states);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;

	return transform;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	// Command current node, if category matches
	if (command.category & getCategory())
		command.action(*this, dt);

	// Command children
	for(Ptr& child : mChildren)
		child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
	return Category::Scene;
}

std::vector<sf::FloatRect> SceneNode::getBoundingRect() const
{
	std::vector<sf::FloatRect> bounds;
	bounds.push_back(sf::FloatRect());
	return bounds;
}


void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const
{
	const std::vector<sf::FloatRect>& rects = getBoundingRect();

	for (auto rect : rects)
	{
		sf::RectangleShape shape;
		shape.setPosition(sf::Vector2f(rect.left, rect.top));
		shape.setSize(sf::Vector2f(rect.width, rect.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);

		target.draw(shape);
	}
}


void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);
	for (Ptr& child : sceneGraph.mChildren)
	{
		checkSceneCollision(*child, collisionPairs);
	}
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node)
		&& !isDestroyed() && !node.isDestroyed())
	{
		collisionPairs.insert(std::minmax(this, &node));
	}

	for (Ptr& child : mChildren)
	{
		child->checkNodeCollision(node, collisionPairs);
	}
}

bool SceneNode::isDestroyed() const
{
	return false;
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	for (auto leftBound : lhs.getBoundingRect())
	{
		for (auto rightBound : rhs.getBoundingRect())
		{
			if (leftBound.intersects(rightBound))
			{
				return true;
			}
		}
	}

	return false;
}

SceneNode* SceneNode::getLastChild()
{
	return mChildren.back().get();
}

const std::vector<SceneNode::Ptr>& SceneNode::getChilds() const
{
	return mChildren;
}

void SceneNode::removeWrecks()
{
	auto wreckfiledBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfiledBegin, mChildren.end());

	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}












