#pragma once
#include "AstElement.hpp"
#include "AstScope.hpp"


class AstControlBlock final : public AstElement
{
public:
	enum type : uint8_t
	{
		CONDITION_BLOCK,
		LOOP_BLOCK,
	};
private:
	std::unique_ptr<AstScope> m_branch;
	std::unique_ptr<AstScope> m_otherBranch;
	const type m_blockType;
public:
	AstControlBlock(std::string_view name, type blockType, AstElement* parent)
		: AstElement(name, ElementType::CONTROL_BLOCK, parent), m_blockType(blockType)
	{
		m_branch = std::make_unique<AstScope>("branch_scope", parent);
	}
	AstControlBlock(AstControlBlock&& block) : AstElement(block.getName(), ElementType::CONTROL_BLOCK, block.getParent()), m_blockType(block.m_blockType)
	{
		m_branch = std::move(block.m_branch);
		m_otherBranch = std::move(block.m_otherBranch);
	}

	void setupOtherBranch()
	{
		m_otherBranch = std::make_unique<AstScope>("other_branch_scope", getParent());
	}
	const std::unique_ptr<AstScope>& getBranchScope() const { return m_branch; }
	void setOtherBranch(std::unique_ptr<AstScope>&& scope) { assert(m_blockType == type::CONDITION_BLOCK); m_otherBranch = std::move(scope); }
	const std::unique_ptr<AstScope>& getOtherBranch() const { return m_otherBranch; }
	const type getBlockType() const { return m_blockType; }
	const std::unique_ptr<AstScope>& getCurrentBranch()
	{
		if(m_otherBranch)
			return m_otherBranch;
		return m_branch;
	}
	virtual ~AstControlBlock() = default;
};