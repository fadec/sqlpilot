#!/usr/bin/env ruby
class Rule
  def self.run
    #puts "#{self}"
    r = self.parse
    #puts "/#{self}"
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
    #puts "  #{ln}"
    ln
  else
#    puts "- #{ln}"
    ungetline
    nil
  end
end

def many(rule)
  if rule.class == Regexp
    while match(rule) do
    end
  else
    while rule.run do
    end
  end
  true
end

def one(rule)
  rule.run
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
      match( /^Bref\s/ ) ||
      match( /^Dbref\s/ ) ||
      match( /^Po\s/ ) ||
      match( /^Layo\s/ ) ||
      match( /^\s+City\s+Code/ )
  end
end

class Trip < Rule
  def self.parse
    (one TripInfo) &&
      (one Blank) &&
      (many Junk) &&
      (many DutyPeriod) &&
      (one TripTotals) &&
      (one TripFooter)
  end
end

class DutyPeriod < Rule
  def self.parse
    (many Flight) &&
      (one Rest) &&
      (one Blank) &&
      (one Junk) &&
      (one Hotel)
  end
end

class Blank < Rule
  def self.parse
    match /^\s*$/
  end
end

class TripInfo < Rule
  def self.parse
    match /^ [a-zA-Z0-9]{5,}/
  end
end

class Flight < Rule
  def self.parse
    (one FlightInfo) &&
      (one Blank) &&
      (one Junk) &&
      (many Crew) &&
      (((match /^Exception/) &&
        (match /.*/)) || true)
  end
end

class FlightInfo < Rule
  def self.parse
    ln = match /^ [0-9]{4}\s/
    puts ln
    ln
  end
end

class Rest < Rule
  def self.parse
    (match /^ DP[0-9]/) &&
      ((match /^\s+[0-9]/ ) || true)
  end
end

class Crew < Rule
  def self.parse
    match /CA01|FO01|FA01/
  end
end

class Hotel < Rule
  def self.parse
    (match /^Hotel\s/) &&
      (((match /^Limo\s/) && (match /^OPS\s/)) || true)
  end
end

class TripTotals < Rule
  def self.parse
    (one Junk) &&
      (match /^\s+[0-9]/)
  end
end

class TripFooter < Rule
  def self.parse
      (match /^Crew Member/) &&
      (match /^Pos /) &&
      (many /Hotel|Limo|Ops/)
  end
end

Schedule.parse
