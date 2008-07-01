#!/usr/bin/env ruby

########################################################################
# Copyright (C) 2008  Sam Danielson                                    #
#                                                                      #
# This file is part of Sqlpilot.                                       #
#                                                                      #
# Sqlpilot is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Sqlpilot is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    #
########################################################################

SHOW_THOUGHTS = false

module Parse
  @@n_current_line = 0
  def run
    n = nline
    puts "#{self}" if SHOW_THOUGHTS
    r = parse
    puts "/#{self}" if SHOW_THOUGHTS
    r || (seekline(n) && nil)
  end

  def parse
    nil
  end

  def lines
    @@lines ||= text.split("\n")
  end

  def text
    @@text ||= STDIN.read
  end

  def getline
    ln = lines[@@n_current_line]
    @@n_current_line += 1
    ln
  end

  def nline
    @@n_current_line
  end

  def seekline(n)
    @@n_current_line = n
    nil
  end

  def match(regex)
    n = nline
    ln = getline
    if ln =~ regex
      puts ln if SHOW_THOUGHTS
      ln
    else
      seekline(n)
      #puts n
      nil
    end
  end

  def many(parser)
    objs = []
    if parser.class == Regexp
      while (obj = match(parser)) do
        objs << obj
      end
    else
      while (obj = parser.run) do
        objs << obj
      end
    end
    objs
  end

  def one(parser)
    if parser.class == Regexp
      match(parser)
    else
      parser.run
    end
  end

end

class Parser
  class <<self
    include Parse
  end
end

class Schedule < Parser
  def self.parse
    many Trip
  end
end

class Junk < Parser
  def self.parse
    if match( /^Pairing\s/ ) ||
        match( /^Flt\s/ ) ||
        match( /^DT\s/ ) ||
        match( /^DH\s/ ) ||
        match( /^Bref\s/ ) ||
        match( /^Dbref\s/ ) ||
        match( /^Po\s/ ) ||
        match( /^Layo\s/ ) ||
        match( /^\s+City\s+Code/ ) ||
        match( /^SS\s/)
      ""
    end
  end
end

class Trip < Parser
  def self.parse
    if (many Junk) &&
        (ti = one TripInfo) &&
        (one Blank) &&
        (many Junk) &&
        (dps = many DutyPeriod) &&
        (one TripTotals) &&
        (one TripFooter)

      {:ident => ti[:ident], :date => "#{ti[:mon]} #{ti[:day]} #{ti[:year]}", :duty_periods => dps}
    end
  end
end

class DutyPeriod < Parser
  def self.parse
    if (fs = many Flight) &&
        (r = one Rest)
      {:flights => fs, :rest => r}
    end
  end
end

class Blank < Parser
  def self.parse
    if match /^\s*$/
      ""
    end
  end
end

class TripInfo < Parser
  def self.parse
    if ln = (match /^ |.\s+[a-zA-Z0-9]{5,}/)
      ident, pos, mon, day, year = ln.gsub(/^.\s+|^\s+/, '').split(/\t/)
      {:ident => ident, :pos => pos, :mon => mon, :day => day, :year => year}
    end
  end
end

class Flight < Parser
  def self.parse
    if (flight = one FlightInfo) &&
        (one Blank) &&
        (match /^Pairing Number\s/) &&
        (crew = many Crew) &&
        (((match /^Exception/) &&
          (match /.*/)) || true)
      flightno, seq, mday, equip, dep, arr, bout, sched, bin, dur, tail = flight.scan /\w+/
      {:flightno => flightno, :seq => seq, :mday => mday, :equip => equip, :dep => dep, :arr => arr, :out => bout, :in => bin, :tail => tail, :crew => crew}
    end
  end
end

class FlightInfo < Parser
  def self.parse
    match /^ [0-9]{4}\s/
  end
end

class Rest < Parser
  def self.parse
    if ((match /^ DP[0-9]/) &&
     ((match /^\s+[0-9]/ ) || true)) &&
      ((one Blank) || true) &&
      (((match /City/) &&
        ((hotel = one Hotel)) || true))
      {:hotel => hotel}
    end
  end
end

class Crew < Parser
  def self.parse
    if (line = match /CA01|FO01|FA01/)
      l = line.split "\t"
      person = [l[2], l[6], l[7].scan(/\w+/).reverse.join(" ")].join " "
      if l[3] =~ /D/
        "DH " + person
      else
        person
      end
    end
  end
end

class Hotel < Parser
  def self.parse
    if (hotel = match /^Hotel\s/) &&
        (((limo = match /^Limo\s/) && (match /^OPS\s/)) || true)
      {:hotel_info => hotel, :limo => limo}
    end
  end
end

class TripTotals < Parser
  def self.parse
    if (one /^Pairing Totals/) &&
        (match /^\s+[0-9]/)
      true
    end
  end
end

class TripFooter < Parser
  def self.parse
    if (match /^Crew Member/) &&
        (match /^Pos /) &&
        (many /Hotel|Limo|Ops/)
      true
    end
  end
end

def make_date(strdate, strmday = nil)
  months = {"Jan" => 1, "Feb" => 2, "Mar" => 3, "Apr" => 4, "May" => 5, "Jun" => 6, "Jul" => 7, "Aug" => 8, "Sep" => 9, "Oct" => 10, "Nov" => 11, "Dec" => 12}
  m, d, y = strdate.scan /\w+/
  m = months[m]
  d = d.to_i
  y = y.to_i + 2000
  if strmday
    mday = strmday.to_i
    m += 1 if d > mday
  else
    mday = d
  end

  t = Time.mktime y, m, mday
  t.strftime "%Y-%m-%d"
end

def csvrow(columns, data)
  columns.map { |c| data[c] }.join(',');
end


columns = [
  :date,
  :leg,
  :fltno,
  :aircraft,
  :type,
  :dep,
  :arr,
  :aout,
  :ain,
  :sout,
  :sin,
  :dur,
  :night,
  :inst,
  :apprch,
  :xc,
  :role,
  :dland,
  :nland,
  :crew,
  :notes,
  :trip,
  :tripdate]

puts columns.join(',')
for trip in Schedule.run
  for duty_period in trip[:duty_periods]
    leg = 0
    for flight in duty_period[:flights]
      leg += 1
      puts csvrow(columns, {
        :date     => (make_date trip[:date], flight[:mday]),
        :leg      => leg,
        :fltno    => flight[:flightno],
        :aircraft => nil,
        :type     => nil,
        :dep      => flight[:dep],
        :arr      => flight[:arr],
        :aout     => nil,
        :ain      => nil,
        :sout     => flight[:out],
        :sin      => flight[:in],
        :dur      => nil,
        :night    => nil,
        :inst     => nil,
        :apprch   => nil,
        :xc       => 1,
        :role     => "CA",
        :dland    => nil,
        :nland    => nil,
        :notes    => nil,
        :crew     => '"' + flight[:crew].join("\n") + '"',
        :trip     => trip[:ident],
        :tripdate => (make_date(trip[:date]) rescue nil)
      })
      
    end
    #if (hotel = duty_period[:rest][:hotel])
      #puts hotel[:hotel_info]
      #if hotel[:limo]
        #puts hotel[:limo]
      #end
    #end
  end
end

