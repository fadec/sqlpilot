#!/usr/bin/env ruby

require 'rubygems'
require 'active_record'
require 'gtk2'

class Object
  def send_methods(methods)
    obj = self
    methods.to_a.flatten.each do |meth|
      obj = obj.send meth
    end
    obj
  end
end

ActiveRecord::Base.establish_connection({
      :adapter => "sqlite3", 
      :dbfile => "db/logbook.db" 
})

class Aircraft < ActiveRecord::Base
  set_table_name :aircraft
  belongs_to :aircraft_type
end

class AircraftType < ActiveRecord::Base
  set_table_name :aircraft_types
  has_many :aircraft
end

class Flight < ActiveRecord::Base
  belongs_to :aircraft
  belongs_to :position
  belongs_to :scheduled_flight
  belongs_to :departure_airport
  belongs_to :arrival_airport
  has_many :through_stops
  has_many :approaches
end

class ScheduledFlight < ActiveRecord::Base
  belongs_to :duty_period
  belongs_to :departure_airport
  belongs_to :arrival_airport
  has_many :flights
end

class DutyPeriod < ActiveRecord::Base
  belongs_to :trip
  has_many :scheduled_flights
end

class Trip < ActiveRecord::Base
  belongs_to :employer
  has_many :duty_periods
end

class Employer < ActiveRecord::Base
  has_many :trips
end

class Airport < ActiveRecord::Base
end

class Approach < ActiveRecord::Base
end

class ApproachType < ActiveRecord::Base
end

class SqlPilot
  def initialize
    @callbacks = Callbacks.new
    @interface = Interface.new(@callbacks)
    @callbacks.interface = @interface
    @callbacks.welcome
  end

  def run
    Gtk.main
  end
end

class Callbacks
  attr_accessor :interface

  def clean_exit
    Gtk.main_quit
  end
  def welcome
    interface.set_pane WelcomePane.new
  end
  def set_pane_aircraft
    interface.set_pane AircraftPane.new
  end
  def set_pane_airports
    interface.set_pane AirportPane.new
  end
end

class Pane
  attr_accessor :container
  
  def initialize
    @container = Gtk::HBox.new
    build_interface
  end

  def build_interface
  end
end

class WelcomePane < Pane
  def initialize
    @container = Gtk::Label.new("Welcome to SQL Pilot")
    @container.show_all
  end
end

class AirportPane < Pane
  def build_interface
    records = Airport.find(:all)
    grid = GridEditor.new records, [[:ident, "Ident"], [:utc_offset, "UTC Offset"]]
    @container.pack_start grid.view
    @container.show_all
  end
end

class AircraftPane < Pane
  def build_interface
    aircraft = Aircraft.find :all
    #grid = GridEditor.new aircraft, [[String, :ident, "Ident"], [String, [:type, :ident], "Type"]
    grid = GridEditor.new aircraft, [[String, :ident, "Ident"]]
    @container.pack_start grid.view
    new_button = Gtk::Button.new("New")
    @container.show_all
  end
end

# GridEditor.new(Aircraft.find(:all), [:ident => "Ident", :aircraft_type => "Type"]
class GridEditor
  attr_accessor :view

  # GTK tree columns
  CLASS_COLUMN = 0
  ID_COLUMN = 1
  DATA_COLUMNS_BEGIN = 2

  # Constructor column arguments
  TYPE = 0
  METHODS = 1
  HEADER = 2

  def initialize(records, columns)
    # Change this to add more columns
    @store = Gtk::TreeStore.new Class, Integer, *columns.map { |c| c[TYPE] }
    
    @view = Gtk::TreeView.new @store

    n = 0
    records.each do |r|
      iter = @store.insert(nil, 1)
      iter[CLASS_COLUMN] = r.class
      iter[ID_COLUMN] = r.id
      n = DATA_COLUMNS_BEGIN
      columns.each do |c|
        iter[n] = get_attribute(r, c[METHODS])
        n += 1
      end
    end

    n = DATA_COLUMNS_BEGIN
    columns.each do |c|
      renderer = Gtk::CellRendererText.new
      renderer.editable = true
      connect_edit_function_object(renderer, c, n)
      column = Gtk::TreeViewColumn.new(c[HEADER], renderer, :text => n)
      @view.append_column column
      n += 1
    end
    @view.show_all
    self
  end

  def connect_edit_function_object(renderer, c, n)
    renderer.signal_connect "edited" do |renderer, path, data|
      iter = @store.get_iter path
      rec = iter[CLASS_COLUMN].find(iter[ID_COLUMN])
      if rec.update_attributes(c[METHOD] => data)
        iter[n] = data
      else
        on_error(rec)
      end
    end
  end

  def edit_function(renderer, path, data)

  end


  def on_error(rec)
    raise 'aaaaaah'
  end
end


class Interface

  attr_accessor :callbacks

  def initialize(callbacks)
    @callbacks = callbacks
    build_interface
    main_window.show_all
  end

  def build_interface
    main_window.add main_window_vbox
    main_window_vbox.pack_start main_menu_bar, false
    main_window_vbox.pack_start content_vbox
    main_window_vbox.pack_end status_bar, false
  end

  def main_window
    return @main_window if @main_window
    window = Gtk::Window.new("SQL Pilot")
    window.signal_connect("delete_event") do
      false
    end
    window.signal_connect("destroy") do
      callbacks.clean_exit
    end
    @main_window = window
  end

  def main_window_vbox
    @main_window_vbox ||= Gtk::VBox.new
  end

  def main_menu_bar
    return @main_menu_bar if @main_menu_bar
    menu_bar = Gtk::MenuBar.new
    file_item = Gtk::MenuItem.new("File")
    menu_bar.append file_item
    file_menu = Gtk::Menu.new
    quit_item = Gtk::MenuItem.new("Quit")
    file_item.submenu = file_menu
    file_menu.append quit_item
    quit_item.signal_connect("activate") do
      callbacks.clean_exit
    end
    flights_item = Gtk::MenuItem.new("Flights")
    menu_bar.append flights_item
    aircraft_item = Gtk::MenuItem.new("Aircraft")
    menu_bar.append aircraft_item
    aircraft_item.submenu = aircraft_menu
    @main_menu_bar = menu_bar
  end

  def aircraft_menu
    return @aircraft_menu if @aircraft_menu
    menu = Gtk::Menu.new
    item = Gtk::MenuItem.new("List")
    item.signal_connect("activate") do
      callbacks.set_pane_aircraft
    end
    menu.append item
    item = Gtk::MenuItem.new("Add")
    item.signal_connect("activate") do
      callbacks.set_pane_airports
    end
    menu.append item
    @aircraft_menu = menu
  end

  def status_bar
    @status_bar ||= Gtk::Statusbar.new
  end

  def content_vbox
    @content_vbox ||= Gtk::VBox.new
  end

  def set_pane(pane)
    content_vbox.remove @content if @content
    @content = pane.container
    content_vbox.pack_start @content
  end

end

def run
  SqlPilot.new.run
end
run if __FILE__ == $0
