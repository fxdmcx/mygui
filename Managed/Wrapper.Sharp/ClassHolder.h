/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __CLASS_ATTRIBUTE_H__
#define __CLASS_ATTRIBUTE_H__

#include "Utility.h"
#include "ITypeHolder.h"

namespace wrapper
{

	class ClassAttribute : public ITypeHolder
	{
	public:
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;
		typedef std::vector<std::string> VectorString;
		typedef std::vector<Member*> VectorMember;

		struct Template
		{
			Template(const std::string& _name, const std::string& _output, const std::string& _type) : name(_name), output(_output), type(_type) { }
			std::string name;
			std::string output;
			std::string type;
		};
		typedef std::vector<Template> VectorTemplate;

	public:
		ClassAttribute(xml::ElementPtr _element)
		{
			mType = _element->findAttribute("type");
			mNamespace = _element->findAttribute("namespace");

			xml::ElementEnumerator child = _element->getElementEnumerator();
			while (child.next())
			{
				if (child->getName() == "Template")
				{
					mTemplates.push_back( Template(child->findAttribute("name"), child->findAttribute("output"), child->findAttribute("type")) );
				}
				if (child->getName() == "AddTemplate")
				{
					mAddTemplates.push_back( PairString(child->findAttribute("name"), child->findAttribute("output")) );
				}
				else if (child->getName() == "HiddenBase")
				{
					mHiddenBase.push_back( child->getContent() );
				}
				else if (child->getName() == "ReplaceTag")
				{
					mPairTag.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
				else if (child->getName() == "ReplaceType")
				{
					mPairType.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
				else if (child->getName() == "ReplaceMethod")
				{
					mPairMethods.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
				else if (child->getName() == "Member")
				{
					mPairMemberData.push_back( PairString(child->findAttribute("name"), child->findAttribute("data")) );
				}
				else if (child->getName() == "TypeInfo")
				{
					mPairTypeInfo.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
			}
		}

		virtual std::string getTypeDescription(const std::string& _type)
		{
			for (VectorPairString::const_iterator item=mPairType.begin(); item!=mPairType.end(); ++item) {
				if (item->first == _type) return item->second;
			}
			return getFullDefinition(_type, mRoot, mNamespace);
		}

		virtual std::string getMemberData(const std::string& _name)
		{
			for (VectorPairString::const_iterator item=mPairMemberData.begin(); item!=mPairMemberData.end(); ++item) {
				if (item->first == _name) return item->second;
			}
			return "";
		}

		virtual std::string getTypeInfo(const std::string& _type)
		{
			for (VectorPairString::const_iterator item=mPairTypeInfo.begin(); item!=mPairTypeInfo.end(); ++item) {
				if (item->first == _type) return item->second;
			}
			return "";
		}

		virtual std::string getMemberName(const std::string& _name)
		{
			if ( ! _name.empty())
			{
				char sim = _name[0];
				if (sim >= 0x61 && sim <= 0x7A)
				{
					sim -= 0x20;
					std::string name = _name;
					name[0] = sim;
					return name;
				}
			}
			return _name;
		}

		void wrap(Compound * _root)
		{
			mRoot = _root;
			createTemplates();
			appendToTemplates();

			// ������ ���� ��������
			VectorMember items;

			// ������� �������� �����
			wrapClass(mType, _root, items);

			// ���� ����� ������� ������� ������
			for (VectorString::iterator item=mHiddenBase.begin(); item!=mHiddenBase.end(); ++item)
			{
				wrapClass(*item, _root, items);
			}

			wrapItems(_root, items);

		}

		void initialise(Compound * _root)
		{
			for (VectorPairString::const_iterator item=mAddTemplates.begin(); item!=mAddTemplates.end(); ++item) {
				std::ofstream onfile;
				onfile.open(item->second.c_str(), std::ios_base::out);
				if ( ! onfile.is_open() ) {
					std::cout << "error open file " << item->first << std::endl;
					continue;
				}
				onfile.close();
			}
		}

	private:

		void wrapItems(Compound * _root, VectorMember& _items)
		{
			std::cout << std::endl;

			// ���� ���������
			for (size_t index=0; index<_items.size(); ++index)
			{
				for (size_t index2=0; index2<_items.size(); ++index2)
				{
					if (index == index2) continue;
					if ( ! _items[index]->postProccesing(_items[index2]) ) continue;

					// ��� ����������, ���������� ������ ���������
					_items.erase(_items.begin() + index2);
					if (index > index2) index--;
					index2--;
				}
			}

			// ������� ������� � ����
			for (size_t index=0; index<_items.size(); ++index)
			{
				wrapMember(_items[index]);
			}

			std::cout << std::endl;

		}

		void wrapClass(const std::string& _name, Compound * _root, VectorMember& _items)
		{
			Compound * item = getCompound("class", _name, _root);
			if (item == nullptr)
			{
				std::cout << mType << " not found" << std::endl;
				return;
			}

			Compound::Enumerator enumerator = item->getEnumerator();
			while (enumerator.next())
			{
				Member* member = getByRef(item->getId(), enumerator->getId());
				if (member == nullptr)
				{
					std::cout << enumerator->getId() << " not found in " << item->getId() << std::endl;
				}
				else
				{
					bool need = true;
					for (VectorPairString::const_iterator item=mPairMethods.begin(); item!=mPairMethods.end(); ++item) {
						if (item->first == member->getName())
						{
							if (item->second == "")
								need = false;
							else
								member->setName(item->second);
							break;
						}
					}

					if (need)
						_items.push_back(member);
				}
			}

			std::cout << std::endl << "class : " << _name << std::endl;

		}

		void createTemplates()
		{
			for (VectorPairString::const_iterator item=mPairTag.begin(); item!=mPairTag.end(); ++item) {
				addTag(item->first, item->second);
			}

			// ������� ����� �������� � ����������� ��
			for (VectorTemplate::const_iterator item=mTemplates.begin(); item!=mTemplates.end(); ++item) {
				std::ifstream infile;
				std::string file = "Data/" + item->type + "/" + item->name;
				infile.open(file.c_str());
				if ( ! infile.is_open() ) {
					std::cout << "error open file " << file << std::endl;
					continue;
				}

				std::ofstream outfile;
				outfile.open(item->output.c_str());
				if ( ! outfile.is_open() ) {
					std::cout << "error open file " << item->output << std::endl;
					infile.close();
					continue;
				}

				std::string read;
				std::string data;

				while (false == infile.eof()) {
					std::getline(infile, read);
					read = replaceTags(read);
					if ( ! data .empty() ) data += "\n";
					data += read;
				}

				outfile << data;

				infile.close();
				outfile.close();
			}
		}

		void appendToTemplates()
		{
			for (VectorPairString::const_iterator item=mPairTag.begin(); item!=mPairTag.end(); ++item) {
				addTag(item->first, item->second);
			}

			// ������� ����� �������� � ����������� ��
			for (VectorPairString::const_iterator item=mAddTemplates.begin(); item!=mAddTemplates.end(); ++item) {
				std::ifstream infile;
				infile.open(item->first.c_str());
				if ( ! infile.is_open() ) {
					std::cout << "error open file " << item->first << std::endl;
					continue;
				}

				std::ofstream outfile;
				outfile.open(item->second.c_str(), std::ios_base::app);
				if ( ! outfile.is_open() ) {
					std::cout << "error open file " << item->second << std::endl;
					infile.close();
					continue;
				}

				std::string read;
				std::string data;

				while (false == infile.eof()) {
					std::getline(infile, read);
					read = replaceTags(read);
					if ( ! data .empty() ) data += "\n";
					data += read;
				}

				// ��� ���������
				if (data.size() > 3) {
					if (data[2] < 32) {
						data[0] = ' ';
						data[1] = ' ';
						data[2] = ' ';
					}
				}

				outfile << data;

				infile.close();
				outfile.close();
			}
		}

		void wrapMember(Member* _member)
		{
			for (VectorTemplate::iterator item=mTemplates.begin(); item!=mTemplates.end(); ++item) {
				_member->insertToTemplate(item->output, this, item->type);
			}
		}

	private:
		std::string mType;
		std::string mNamespace;
		VectorTemplate mTemplates;
		VectorPairString mAddTemplates;
		VectorPairString mPairTag;
		VectorPairString mPairType;
		VectorPairString mPairMethods;
		VectorPairString mPairMemberData;
		VectorPairString mPairTypeInfo;
		VectorString mHiddenBase;
		Compound * mRoot;
	};

} // namespace wrapper

#endif // __CLASS_ATTRIBUTE_H__
