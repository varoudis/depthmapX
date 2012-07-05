// sala - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2011-2012, Tasos Varoudis

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef __FILEPROPERTIES_H__
#define __FILEPROPERTIES_H__

class FileProperties
{
protected:
   pstring m_create_person;
   pstring m_create_organization;
   pstring m_create_date;
   pstring m_create_program;
   pstring m_title;
   pstring m_location;
   pstring m_description;
public:
   FileProperties() {;}
   virtual ~FileProperties() {;}
   //
   void setProperties(const pstring& person, const pstring& organization, const pstring& date, const pstring& program)
   { m_create_person = person; m_create_organization = organization; m_create_date = date; m_create_program = program; }
   void setTitle(const pstring& title)
   { m_title = title; }
   void setLocation(const pstring& location)
   { m_location = location; }
   void setDescription(const pstring& description)
   { m_description = description; }
   //
   const pstring& getPerson() const
   { return m_create_person; }
   const pstring& getOrganization() const
   { return m_create_organization; }
   const pstring& getDate() const
   { return m_create_date; }
   const pstring& getProgram() const
   { return m_create_program; }
   const pstring& getTitle() const
   { return m_title; }
   const pstring& getLocation() const
   { return m_location; }
   const pstring& getDescription() const
   { return m_description; }
   //
   bool read( ifstream& stream, int version );
   bool write( ofstream& stream );
};

inline bool FileProperties::read(ifstream& stream, int version)
{
   m_create_person.read(stream);
   m_create_organization.read(stream);
   m_create_date.read(stream);
   m_create_program.read(stream);
   m_title.read(stream);
   m_location.read(stream);
   m_description.read(stream);

   return true;
}

inline bool FileProperties::write(ofstream& stream)
{
   m_create_person.write(stream);
   m_create_organization.write(stream);
   m_create_date.write(stream);
   m_create_program.write(stream);
   m_title.write(stream);
   m_location.write(stream);
   m_description.write(stream);

   return true;
}

#endif

