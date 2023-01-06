#include "yaAnimator.h"
#include "yaGameObject.h"
#include "yaImage.h"
#include "yaCamera.h"
#include "yaResources.h"

namespace ya
{
	Animator::Animator()
		: Component(eComponentType::Aimator)
		, mPlayAnimaion(nullptr)
		, mbLoop(false)
		, mSPriteSheet(nullptr)
		, mbAni(false)

	{
		//mImage = Resources::Load<Image>(L"Player", L"..\\Resources\\Image\\Player.bmp");
	}

	Animator::~Animator()
	{
		//std::map<std::wstring, Animation*> mAnimations;
		for (auto iter : mAnimations)
		{
			delete iter.second;
		}

		//std::map<std::wstring, Events*> mEvents;
		for (auto iter : mEvents)
		{
			delete iter.second;
		}
	}

	void Animator::Tick()
	{
		if (mPlayAnimaion != nullptr)
		{
			mPlayAnimaion->Tick();

			if (mbLoop && mPlayAnimaion->isComplete())
			{
				Animator::Events* events
					= FindEvents(mPlayAnimaion->GetName());
				if (events != nullptr)
					events->mCompleteEvent();

				mPlayAnimaion->Reset();
			}
		}
	}

	void Animator::Render(HDC hdc)
	{		
		if (mPlayAnimaion != nullptr)
		{
			if(mbAni)			
			mPlayAnimaion->SetAnirender(true);
						
			mPlayAnimaion->Render(hdc);
		}
	}

	std::wstring Animator::CreateAniamtionKey(std::wstring path)
	{
		std::wstring keyString = path;

		// ��ϸ��̼� ���� �̸� ����
		UINT pos = (UINT)keyString.find_last_of(L"\\");
		std::wstring tail = keyString.substr(pos + 1, keyString.length());
		keyString = keyString.substr(0, pos);

		// �ִϸ��̼� ������Ʈ �̸� ����
		pos = (UINT)keyString.find_last_of(L"\\");
		std::wstring head = keyString.substr(pos + 1, keyString.length());
		keyString = head + tail;

		return keyString;
	}

	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter = mAnimations.find(name);
		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;
	}

	void Animator::CreateAnimation(const std::wstring& name, Texture* image
		, Vector2 leftTop, Vector2 size, Vector2 offset
		, UINT spriteLegth, float duration, bool bAffectedCamera)
	{
		Animation* animation = FindAnimation(name);
		if (animation != nullptr)
		{
			MessageBox(nullptr, L"�ߺ� Ű �ִϸ��̼� ����", L"�ִϸ��̼� ���� ����!", MB_OK);
			return;
		}

		animation = new Animation();
		animation->Create(image, leftTop, size, offset
			, spriteLegth, duration, bAffectedCamera);

		animation->SetName(name);
		animation->SetAnimator(this);

		mAnimations.insert(std::make_pair(name, animation));

		Events* events = new Events();
		mEvents.insert(std::make_pair(name, events));
	}

	void Animator::PushAnimation(const std::wstring& name, Texture* image, std::vector<Sprite> frm, bool bAffectedCamera)
	{
		Animation* animation = FindAnimation(name);
		if (animation != nullptr)
		{
			MessageBox(nullptr, L"�ߺ� Ű �ִϸ��̼� ����", L"�ִϸ��̼� ���� ����!", MB_OK);
			return;
		}
	

		animation = new Animation();
		animation->CreateAnima(name, image, frm, bAffectedCamera);

		animation->SetName(name);
		animation->SetAnimator(this);

		mAnimations.insert(std::make_pair(name, animation));

		Events* events = new Events();
		mEvents.insert(std::make_pair(name, events));
	}

	void Animator::CreateAnimations(const std::wstring& path
		, const std::wstring& name
		, Vector2 offset
		, float duration)
	{
		UINT width = 0;
		UINT height = 0;
		UINT fileCount = 0;

		std::filesystem::path fs(path);
		std::vector<Texture*> images;
		int fileIndex = 0;
		for (auto& p : std::filesystem::recursive_directory_iterator(path))
		{
			std::wstring fileName = p.path().filename();;
			std::wstring key = CreateAniamtionKey(path) + std::to_wstring(fileIndex++);
			std::wstring fullName = path + L"\\" + fileName;

			Texture* image = Resources::Load<Texture>(key, fullName);
			images.push_back(image);

			if (width < image->GetWidth())
				width = image->GetWidth();

			if (height < image->GetHeight())
				height = image->GetHeight();

			fileCount++;
		}

		mSPriteSheet = Texture::Create(name, width * fileCount, height);
		int index = 0;
		for (Texture* image : images)
		{
			BitBlt(mSPriteSheet->GetDC(), width * index, 0, image->GetWidth(), image->GetHeight()
				, image->GetDC(), 0, 0, SRCCOPY);
			index++;
		}

		CreateAnimation(name, mSPriteSheet
			, Vector2(0.0f, 0.0f), Vector2((float)width, (float)height)
			, offset, fileCount, duration);
	}

	void Animator::Play(const std::wstring& name, bool bLoop)
	{
		Animator::Events* events = FindEvents(name);
		if (events != nullptr)
			events->mStartEvent();

		Animation* prevAnimation = mPlayAnimaion;
		mPlayAnimaion = FindAnimation(name);
		mPlayAnimaion->Reset();
		mbLoop = bLoop;

		if (prevAnimation != mPlayAnimaion)
		{
			if (events != nullptr)
				events->mEndEvent();
		}
	}
	Animator::Events* Animator::FindEvents(const std::wstring key)
	{
		std::map<std::wstring, Events*>::iterator iter = mEvents.find(key);
		if (iter == mEvents.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	std::function<void()>& Animator::GetStartEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);

		return events->mStartEvent.mEvent;
	}

	std::function<void()>& Animator::GetCompleteEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);

		return events->mCompleteEvent.mEvent;
	}

	std::function<void()>& Animator::GetEndEvent(const std::wstring key)
	{
		Events* events = FindEvents(key);

		return events->mEndEvent.mEvent;
	}


	void Animator::SaveAnima(const std::wstring& name, const std::wstring& path, Texture* image, std::vector<Sprite> frm, bool bAffectedCamera)
	{

		//���̺� �� ���� �׿� ������� ���� �Ҷ� ���
		//std::wstring strFilepath = L"..\\Resources\\AnimationSaveFiles\\";
		//	strFilepath += name;

		//���̺� ���� �����Ҷ� ���
		std::wstring strFilepath = path;

			// ���� �����
		FILE* pFile = nullptr;
		errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"wb");

		if (nullptr == pFile)
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Animation Save ����, Error Number : %d", iErrNum);
			MessageBox(nullptr, szStr, L"���� ���� ����", MB_OK);
			return;
		}

		mAnifrm = frm;

		// ������ ���� ����
		size_t iFrmCount = mAnifrm.size();
		fwrite(&iFrmCount, sizeof(size_t), 1, pFile);
		fwrite(mAnifrm.data(), sizeof(Sprite), mAnifrm.size(), pFile);

		// Animation Ű�� ����
		//SaveWString(GetName(), pFile);		
		SaveWString(name, pFile);

		// ��Ʋ�� �ؽ��� Ű�� ����	
		SaveWString(image->GetKey(), pFile);
		
		// ��Ʋ�� �ؽ��� ����� ����
		SaveWString(image->GetPath(), pFile);
		//SaveWString(path, pFile);

		fclose(pFile);

		
	}

	void Animator::LoadAnima(const std::wstring& name)
	{

		//std::wstring strFilepath = L"..\\Resources\\AnimationSaveFiles\\";
		//strFilepath += name;

		std::wstring strFilepath = name;

		// ���� �����
		FILE* pFile = nullptr;
		errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"rb");

		if (nullptr == pFile)
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Animation Load ����, Error Number : %d", iErrNum);
			MessageBox(nullptr, szStr, L"���� �ҷ����� ����", MB_OK);
			return;
		}

		
		// ������ ���� �б�
		size_t iFrmCount = 0;
		fread(&iFrmCount, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < iFrmCount; ++i)
		{
			
			Sprite frm = {};			
			fread(&frm, sizeof(Sprite), 1, pFile);
								
			mAnifrm.push_back(frm);
		}

		// Animation �̸� �б�
		std::wstring strName;
		LoadWString(strName, pFile);
		SetName(strName);

		// ��Ʋ�� �ؽ��� Ű, ��� �б�
		std::wstring strKey, strRelativePath;
		LoadWString(strKey, pFile);
		LoadWString(strRelativePath, pFile);

		mSPriteSheet = Resources::Load<Texture>(strKey, strRelativePath);
		PushAnimation(strName, mSPriteSheet, mAnifrm);
		//m_pAtlas = CResMgr::GetInst()->LoadTexture(strKey, strRelativePath);

		fclose(pFile);


	}


}
