#include "ITGSceneItem.h"

std::shared_ptr<ITGSceneItem> TGSceneItemImpl::GetParent()
{
	return mParent;
}

std::shared_ptr<ITGSceneItem> TGSceneItemImpl::GetChildAt(int index)
{
	if (index < 0 || index >= mChildrenItems.size()) return nullptr;

	return mChildrenItems[index];
}

std::vector<std::shared_ptr<ITGSceneItem>> TGSceneItemImpl::GetChildren()
{
	return mChildrenItems;
}

std::string TGSceneItemImpl::GetItemName()
{
	return mItemName;
}

ETGSceneItemType TGSceneItemImpl::GetItemType()
{
	return mItemType;
}

void TGSceneItemImpl::AddChild(std::shared_ptr<ITGSceneItem> child)
{
	mChildrenItems.push_back(child);
}

void TGSceneItemImpl::RemoveChildAt(int index)
{
	if (index < 0 || index >= mChildrenItems.size()) return;

	mChildrenItems.erase(mChildrenItems.begin() + index);
}

void TGSceneItemImpl::SetParent(std::shared_ptr<ITGSceneItem> parent)
{
	mParent = parent;
}

void TGSceneItemImpl::ClearChildren()
{
	mChildrenItems.clear();
}

void TGSceneItemImpl::SetVisibility(bool visibility)
{
	mVisibleType = visibility ? ETGSceneItemVisibleType::ETGSceneItemVisibleType_AllVisible : ETGSceneItemVisibleType::ETGSceneItemVisibleType_NotVisible;

	for (int i = 0; i < mChildrenItems.size(); i++)
	{
		mChildrenItems[i]->SetVisibility(visibility);
	}

	std::shared_ptr<TGSceneItemImpl> currSceneItem = SharedFromThis();

	while (currSceneItem)
	{
		currSceneItem->mVisibleType = currSceneItem->FindChildrenVisibilityStatus(currSceneItem);

		currSceneItem = std::static_pointer_cast<TGSceneItemImpl>(currSceneItem->GetParent());
	}
}

ETGSceneItemVisibleType TGSceneItemImpl::GetVisibility()
{
	return mVisibleType;
}

void TGSceneItemImpl::Tick(double deltaTime)
{
	
}

void TGSceneItemImpl::SetTickPriority(ETGTickPriority tickPriority)
{
	mTickPriority = tickPriority;
}

ETGTickPriority TGSceneItemImpl::GetTickPriority()
{
	return mTickPriority;
}

std::shared_ptr<TGSceneItemImpl> TGSceneItemImpl::SharedFromThis()
{
	return std::shared_ptr<TGSceneItemImpl>(this);
}

int TGSceneItemImpl::GetChildrenNumber()
{
	return mChildrenItems.size();
}

ETGSceneItemVisibleType TGSceneItemImpl::FindChildrenVisibilityStatus(std::shared_ptr<ITGSceneItem> sceneItem)
{
	if (sceneItem == nullptr) return ETGSceneItemVisibleType_AllVisible;

	int visibleChildNumber = 0;

	for (int i = 0; i < sceneItem->GetChildrenNumber(); i++)
	{
		ETGSceneItemVisibleType childVisibility = sceneItem->GetChildAt(i)->GetVisibility();

		if (childVisibility == ETGSceneItemVisibleType_PartVisible)
		{
			return ETGSceneItemVisibleType_PartVisible;
		}
		else if (childVisibility == ETGSceneItemVisibleType_AllVisible)
		{
			visibleChildNumber++;
		}
	}

	if (visibleChildNumber == sceneItem->GetChildrenNumber())
	{
		return ETGSceneItemVisibleType_AllVisible;
	}
	else if (visibleChildNumber == 0)
	{
		return ETGSceneItemVisibleType_NotVisible;
	}

	return ETGSceneItemVisibleType_PartVisible; 
}
