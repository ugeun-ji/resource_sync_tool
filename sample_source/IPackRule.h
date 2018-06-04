#pragma once

namespace rsc
{
	// ��ŷ �� �߰��Ǵ� ���� �Ǵ� ���� ����.
	struct PackAddTarget
	{
		std::string		strRelPath;
		bool			includeSubFolderOK;
	};

	using PackAddTargetPtr = std::shared_ptr<PackAddTarget>;

	// �� �м� ����.
	struct PackAnalysis
	{
		std::vector<PathPtr>			targetList;
		std::string						stri3PackName;			// �� ���ϸ�.
		std::vector<PackAddTargetPtr>	addList;				// �߰� ��� �����.
		std::vector<std::string>		strRemoveRelPathList;	// ���� ��� �����.
		std::vector<std::string>		strEncryptRelPathList;	// ��ȣȭ ��� �����.
		bool							checksumOK;				// �� üũ��.
	};

	void ClearPackAnalysis(PackAnalysis& src);

	struct PackRuleCallback
	{
		virtual void OnPackComplete(const std::vector<PathPtr>& srcPackTgtList,
									const Path& srcWorkdir,
									const Path& dstWorkdir,
									const PackAnalysis& analysis,
									const bool success,
									const wchar_t* errMessage)
		{}

		virtual void OnCopyComplete(const std::vector<PathPtr>& srcCopyOrFailList,
									const Path& srcWorkdir,
									const Path& dstWorkdir, 
									const bool success,
									const wchar_t* errMessage)
		{}

		virtual ~PackRuleCallback() {}
	};

	using PackRuleCallbackPtr = std::shared_ptr<PackRuleCallback>;

	class IPackRule
	{
	public:
		IPackRule(const std::vector<PathPtr>& srcList,	// ��ŷ ��� ���� ���ϵ�.
				  const Path& srcWorkdir, 
				  const Path& dstWorkdir);
		
		virtual ~IPackRule() = 0;

		void operator()(bool bCheckDuplicatedFile);

		virtual bool Analyze(PackAnalysis& outAnalysis, std::vector<PathPtr>& outCopyList) = 0;

		void				SetCallback(PackRuleCallbackPtr cb)	{ m_callback = std::move(cb);	}
		PackRuleCallback*	GetCallback() const					{ return m_callback.get();		}
	
	protected:
		void _Copy(const std::vector<PathPtr>& copyList);
		bool _Pack(const PackAnalysis& analysis, bool bCheckDuplicatedFile);
		
		std::vector<PathPtr> m_srcPackTgtList;		// ���� ���� �� �� ��� ����.
		std::vector<PathPtr> m_srcCopyTgtList;		// ���� ���� �� ī�� ��� ����.

		const Path m_srcWorkdir;
		const Path m_dstWorkdir;
		
		PackRuleCallbackPtr m_callback;
	};

	using PackRulePtr = std::shared_ptr < IPackRule > ;
}