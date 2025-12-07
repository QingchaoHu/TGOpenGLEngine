#pragma once
#include <memory>
#include <vector>
#include <string>

enum ETGSceneItemType
{
	// 摄像机
	ETGSceneItemType_Camera = 0,

	// 物体
	ETGSceneItemType_Primitive = 1,

	// 体积
	ETGSceneItemType_Volumn = 2,

	// 集合对象
	ETGSceneItemType_Collection = 3,

	// 未知
	ETGSceneItemType_Unknown = 99
};

enum ETGSceneItemVisibleType
{
	// 全可见
	ETGSceneItemVisibleType_AllVisible = 0,

	// 部分可见
	ETGSceneItemVisibleType_PartVisible = 1,

	// 全不可见
	ETGSceneItemVisibleType_NotVisible = 2
};

enum ETGTickPriority
{
	ETGTickPriority_Level0 = 0,
	ETGTickPriority_Level1 = 1,
	ETGTickPriority_Level2 = 2,
	ETGTickPriority_Level3 = 3,
	ETGTickPriority_Level4 = 4,
	ETGTickPriority_Level5 = 5,
	ETGTickPriority_Level6 = 6,
	ETGTickPriority_Level7 = 7,
	ETGTickPriority_Level8 = 8,
	ETGTickPriority_Level9 = 9,
};

class ITGTickable
{
public:
	virtual void Tick(double deltaTime) = 0;

	virtual void SetTickPriority(ETGTickPriority tickPriority) = 0;

	virtual ETGTickPriority GetTickPriority() = 0;
};

class ITGSceneItem : public ITGTickable
{
public:
	virtual std::shared_ptr<ITGSceneItem> GetParent() = 0;

	virtual std::shared_ptr<ITGSceneItem> GetChildAt(int index) = 0;

	virtual std::vector<std::shared_ptr<ITGSceneItem>> GetChildren() = 0;

	virtual std::string GetItemName() = 0;

	virtual ETGSceneItemType GetItemType() = 0;

	virtual void AddChild(std::shared_ptr<ITGSceneItem> child) = 0;

	virtual void RemoveChildAt(int index) = 0;

	virtual void SetParent(std::shared_ptr<ITGSceneItem> parent) = 0;

	virtual void ClearChildren() = 0;

	virtual void SetVisibility(bool visibility) = 0;

	virtual ETGSceneItemVisibleType GetVisibility() = 0;

	virtual int GetChildrenNumber() = 0;
};

class TGSceneItemImpl : public ITGSceneItem
{
public:
	virtual std::shared_ptr<ITGSceneItem> GetParent() override;

	virtual std::shared_ptr<ITGSceneItem> GetChildAt(int index) override;

	virtual std::vector<std::shared_ptr<ITGSceneItem>> GetChildren() override;

	virtual std::string GetItemName() override;

	virtual ETGSceneItemType GetItemType() override;

	virtual void AddChild(std::shared_ptr<ITGSceneItem> child) override;

	virtual void RemoveChildAt(int index) override;

	virtual void SetParent(std::shared_ptr<ITGSceneItem> parent) override;

	virtual void ClearChildren() override;

	virtual void SetVisibility(bool visibility) override;

	virtual ETGSceneItemVisibleType GetVisibility() override;

	virtual void Tick(double deltaTime) override;

	virtual void SetTickPriority(ETGTickPriority tickPriority) override;

	virtual ETGTickPriority GetTickPriority() override;

	virtual std::shared_ptr<TGSceneItemImpl> SharedFromThis();

	virtual int GetChildrenNumber() override;

protected:
	virtual ETGSceneItemVisibleType FindChildrenVisibilityStatus(std::shared_ptr<ITGSceneItem> sceneItem);

protected:
	std::vector<std::shared_ptr<ITGSceneItem>> mChildrenItems;

	std::shared_ptr<ITGSceneItem> mParent = nullptr;

	std::string mItemName = "DefaultItem";

	ETGSceneItemType mItemType = ETGSceneItemType_Unknown;

	ETGSceneItemVisibleType mVisibleType = ETGSceneItemVisibleType_AllVisible;

	ETGTickPriority mTickPriority = ETGTickPriority_Level5;
};