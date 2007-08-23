#!/usr/bin/env ruby

require 'rubygems'
require 'active_record'
require 'gtk2'

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
end

class Pane
  attr_accessor :widget
end

class WelcomePane < Pane
  def initialize
    @widget = Gtk::Label.new("Welcome to SQL Pilot")
    @widget.show_all

  end
end

class AircraftPane < Pane
  def initialize
    @widget = Gtk::HBox.new
    build_interface
  end

  def build_interface
    store = Gtk::TreeStore.new(Integer, String, String)
    view = Gtk::TreeView.new store
    ident_renderer = Gtk::CellRendererText.new
    ident_renderer.editable = true
    type_renderer = Gtk::CellRendererText.new
    type_renderer.editable = true
    ident_column = Gtk::TreeViewColumn.new("Ident", ident_renderer, :text => 1)
    type_column = Gtk::TreeViewColumn.new("Type", type_renderer, :text => 2)
    view.append_column ident_column
    view.append_column type_column
    ident_renderer.signal_connect("edited") do |renderer, path, data|
      iter = store.get_iter path
      iter[1] = data
      Aircraft.find(iter[0]).update_attributes(:ident => data)
    end
    type_renderer.signal_connect("edited") { |a, b, c| puts a; puts b; puts c; }
    Aircraft.find(:all).each do |a|
      iter = store.insert(nil,1)
      iter[0] = a.id
      iter[1] = a.ident
      iter[2] = a.aircraft_type.name
    end
    @widget.pack_start view
    new_button = Gtk::Button.new("New")
    @widget.show_all
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
    @content = pane.widget
    content_vbox.pack_start @content
  end

end

def run
  SqlPilot.new.run
end
run if __FILE__ == $0
