#!/usr/bin/env ruby
class Rule
  def self.run
    puts "#{self}"
    r = self.parse
    puts "/#{self}"
    r
  end
  def self.parse
    nil
  end
end

def lines
  @@lines ||= text.split("\n")
end

def text
  @@text ||= File.read "/home/sam/schedule.txt"
end

def getline
  @@n_current_line ||= 0
  ln = lines[@@n_current_line]
  @@n_current_line += 1
  ln
end

def ungetline
  @@n_current_line -= 1
  lines[@@n_current_line]
end

def match(regex)
  ln = getline
  if ln =~ regex
    puts "  #{ln}"
    ln
  else
#    puts "- #{ln}"
    ungetline
    nil
  end
end

def many(rule)
  while rule.run do
  end
end

def one(rule)
  rule.run
end

def maybe(rule)
  
end

def oneOf(rules)

end

class Schedule < Rule
  def self.parse
    many Junk
    many Trip
  end
end

class Junk < Rule
  def self.parse
    match( /^Pairing\s/ ) ||
      match( /^Flt\s/ ) ||
      match( /^DT\s/ ) ||
      match( /^DH\s/ ) ||
      match(  /^Bref\s/ ) ||
      match(  /^Dbref\s/ ) ||
      match(  /^Po\s/ ) ||
      match(  /^Layo\s/ )
  end
end

class Trip < Rule
  def self.parse
    (one TripInfo) &&
      (one Blank) &&
      (many Junk) &&
      (many DutyPeriod)
  end
end

class DutyPeriod
  def self.parse
    many Flight
  end
end

class Blank < Rule
  def self.parse
    match /\s.*/
  end
end

class TripInfo < Rule
  def self.parse
    match /^ [a-zA-Z0-9]{5,}/
  end
end

class Flight < Rule
  def self.parse
    one FlightInfo
    one Blank
    many Crew
  end
end

class FlightInfo < Rule
  def self.parse
    match /^ [0-9]{4}\s/
  end
end

class Rest < Rule
  def self.parse
    match /^ DP[0-9]/
  end
end

class Crew < Rule
  def self.parse
    match /^[0-9a-zA-Z]{5,}/
  end
end

class Hotel < Rule
  def self.parse
    match /^Hotel\s/
  end
end

Schedule.parse
