"""
GUI Application Framework

This module provides a comprehensive GUI application framework for building cross-platform
desktop applications with native look and feel.

Features:
- Window and dialog management
- Widget toolkit (buttons, text fields, lists, etc.)
- Event handling system
- Layout managers
- Theme support
- Menu and toolbar system
- File dialogs and message boxes
- Drag and drop support
- Canvas for custom drawing
- MVC pattern implementation
"""

import os
import sys
import time
import random
import math
import uuid
from typing import List, Dict, Tuple, Optional, Union, Any, Callable
from datetime import datetime
from enum import Enum

# Try to import tkinter for GUI
try:
    import tkinter as tk
    from tkinter import ttk, messagebox, filedialog, simpledialog
    from tkinter import font as tkfont
    from tkinter import colorchooser
    TKINTER_AVAILABLE = True
except ImportError:
    TKINTER_AVAILABLE = False

# Try to import PIL for image handling
try:
    from PIL import Image, ImageTk, ImageDraw, ImageFont, ImageFilter
    PIL_AVAILABLE = True
except ImportError:
    PIL_AVAILABLE = False


class EventType(Enum):
    """Enumeration of GUI event types."""
    CLICK = "click"
    DOUBLE_CLICK = "double_click"
    KEY_PRESS = "key_press"
    KEY_RELEASE = "key_release"
    MOUSE_MOVE = "mouse_move"
    MOUSE_ENTER = "mouse_enter"
    MOUSE_LEAVE = "mouse_leave"
    FOCUS_GAIN = "focus_gain"
    FOCUS_LOSE = "focus_lose"
    RESIZE = "resize"
    MOVE = "move"
    CLOSE = "close"
    CUSTOM = "custom"


class Alignment(Enum):
    """Enumeration of alignment options."""
    LEFT = "left"
    CENTER = "center"
    RIGHT = "right"
    TOP = "top"
    MIDDLE = "middle"
    BOTTOM = "bottom"


class Widget:
    """Base class for all GUI widgets."""
    
    def __init__(self, parent=None, id=None, **kwargs):
        """
        Initialize the widget.
        
        Args:
            parent: Parent widget
            id: Widget ID
            **kwargs: Additional widget properties
        """
        self.parent = parent
        self.id = id or str(uuid.uuid4())
        self.properties = kwargs
        self.event_handlers = {}
        self.children = []
        self.visible = True
        self.enabled = True
        self.position = kwargs.get('position', (0, 0))
        self.size = kwargs.get('size', (100, 30))
        self.alignment = kwargs.get('alignment', Alignment.LEFT)
        self.style = kwargs.get('style', {})
        self.tooltip = kwargs.get('tooltip', None)
        
        # Add this widget to parent's children if parent exists
        if parent:
            parent.children.append(self)
    
    def add_event_handler(self, event_type, handler):
        """
        Add an event handler for this widget.
        
        Args:
            event_type: Type of event
            handler: Handler function
        """
        if event_type not in self.event_handlers:
            self.event_handlers[event_type] = []
        
        self.event_handlers[event_type].append(handler)
    
    def remove_event_handler(self, event_type, handler):
        """
        Remove an event handler for this widget.
        
        Args:
            event_type: Type of event
            handler: Handler function
        """
        if event_type in self.event_handlers:
            if handler in self.event_handlers[event_type]:
                self.event_handlers[event_type].remove(handler)
    
    def trigger_event(self, event_type, event_data=None):
        """
        Trigger an event for this widget.
        
        Args:
            event_type: Type of event
            event_data: Event data
        """
        if event_type in self.event_handlers:
            for handler in self.event_handlers[event_type]:
                try:
                    handler(self, event_type, event_data)
                except Exception as e:
                    print(f"Error in event handler: {str(e)}")
        
        # Propagate event to parent if needed
        if self.parent:
            self.parent.trigger_event(event_type, event_data)
    
    def set_property(self, name, value):
        """
        Set a widget property.
        
        Args:
            name: Property name
            value: Property value
        """
        self.properties[name] = value
    
    def get_property(self, name, default=None):
        """
        Get a widget property.
        
        Args:
            name: Property name
            default: Default value if property doesn't exist
            
        Returns:
            Property value
        """
        return self.properties.get(name, default)
    
    def show(self):
        """Show the widget."""
        self.visible = True
        self.trigger_event(EventType.RESIZE, {'visible': True})
    
    def hide(self):
        """Hide the widget."""
        self.visible = False
        self.trigger_event(EventType.RESIZE, {'visible': False})
    
    def enable(self):
        """Enable the widget."""
        self.enabled = True
    
    def disable(self):
        """Disable the widget."""
        self.enabled = False
    
    def get_bounds(self):
        """
        Get the bounds of the widget.
        
        Returns:
            Tuple of (x, y, width, height)
        """
        x, y = self.position
        width, height = self.size
        return (x, y, width, height)
    
    def set_bounds(self, x, y, width, height):
        """
        Set the bounds of the widget.
        
        Args:
            x: X position
            y: Y position
            width: Width
            height: Height
        """
        self.position = (x, y)
        self.size = (width, height)
        self.trigger_event(EventType.RESIZE, {
            'x': x, 'y': y, 'width': width, 'height': height
        })
    
    def get_style(self, name, default=None):
        """
        Get a style property.
        
        Args:
            name: Style property name
            default: Default value if property doesn't exist
            
        Returns:
            Style property value
        """
        # Check widget-specific style
        if name in self.style:
            return self.style[name]
        
        # Check parent style
        if self.parent:
            return self.parent.get_style(name, default)
        
        return default
    
    def set_style(self, name, value):
        """
        Set a style property.
        
        Args:
            name: Style property name
            value: Style property value
        """
        self.style[name] = value
    
    def render(self):
        """Render the widget (abstract method)."""
        pass
    
    def __str__(self):
        """String representation of the widget."""
        return f"{self.__class__.__name__}(id={self.id}, visible={self.visible}, enabled={self.enabled})"


class Window(Widget):
    """Window widget for creating application windows."""
    
    def __init__(self, title="Window", width=800, height=600, **kwargs):
        """
        Initialize the window.
        
        Args:
            title: Window title
            width: Window width
            height: Window height
            **kwargs: Additional window properties
        """
        super().__init__(None, **kwargs)
        self.title = title
        self.size = (width, height)
        self.position = kwargs.get('position', (100, 100))
        self.resizable = kwargs.get('resizable', True)
        self.modal = kwargs.get('modal', False)
        self.min_size = kwargs.get('min_size', (200, 100))
        self.max_size = kwargs.get('max_size', (2000, 1500))
        self.decorated = kwargs.get('decorated', True)
        self.always_on_top = kwargs.get('always_on_top', False)
        self.alpha = kwargs.get('alpha', 1.0)
        self.menu_bar = None
        self.tool_bar = None
        self.status_bar = None
        self.icon = None
        self.cursor = kwargs.get('cursor', 'arrow')
        
        # Initialize the underlying GUI toolkit window if available
        self._tk_window = None
        self._create_tk_window()
    
    def _create_tk_window(self):
        """Create the underlying Tkinter window."""
        if not TKINTER_AVAILABLE:
            return
        
        # Create the root window if it doesn't exist yet
        if hasattr(self, '_tk_root') and self._tk_root:
            self._tk_window = self._tk_root
        else:
            self._tk_window = tk.Tk()
            self._tk_root = self._tk_window
        
        # Set window properties
        self._tk_window.title(self.title)
        self._tk_window.geometry(f"{self.size[0]}x{self.size[1]}+{self.position[0]}+{self.position[1]}")
        self._tk_window.minsize(*self.min_size)
        self._tk_window.maxsize(*self.max_size)
        
        # Set window icon if provided
        if self.icon and PIL_AVAILABLE:
            try:
                icon_image = Image.open(self.icon)
                self._tk_window.iconphoto(False, ImageTk.PhotoImage(icon_image))
            except Exception as e:
                print(f"Error setting window icon: {str(e)}")
        
        # Set window alpha if supported
        try:
            self._tk_window.attributes('-alpha', self.alpha)
        except:
            pass  # Alpha might not be supported on all platforms
        
        # Set always on top if needed
        if self.always_on_top:
            self._tk_window.attributes('-topmost', True)
        
        # Set cursor
        self._tk_window.config(cursor=self.cursor)
        
        # Bind event handlers
        self._tk_window.protocol("WM_DELETE_WINDOW", self._on_close)
        self._tk_window.bind("<Configure>", self._on_resize)
        self._tk_window.bind("<FocusIn>", self._on_focus_gain)
        self._tk_window.bind("<FocusOut>", self._on_focus_lose)
        
        # Create a frame for the content
        self._content_frame = ttk.Frame(self._tk_window)
        self._content_frame.pack(fill=tk.BOTH, expand=True)
    
    def _on_close(self):
        """Handle window close event."""
        self.trigger_event(EventType.CLOSE, {})
        self._tk_window.destroy()
    
    def _on_resize(self, event):
        """Handle window resize event."""
        width = event.width
        height = event.height
        self.size = (width, height)
        self.trigger_event(EventType.RESIZE, {'width': width, 'height': height})
    
    def _on_focus_gain(self, event):
        """Handle window focus gain event."""
        self.trigger_event(EventType.FOCUS_GAIN, {})
    
    def _on_focus_lose(self, event):
        """Handle window focus lose event."""
        self.trigger_event(EventType.FOCUS_LOSE, {})
    
    def set_title(self, title):
        """
        Set the window title.
        
        Args:
            title: New window title
        """
        self.title = title
        if self._tk_window:
            self._tk_window.title(title)
    
    def get_title(self):
        """
        Get the window title.
        
        Returns:
            Window title
        """
        return self.title
    
    def set_icon(self, icon_path):
        """
        Set the window icon.
        
        Args:
            icon_path: Path to the icon file
        """
        self.icon = icon_path
        if self._tk_window and PIL_AVAILABLE:
            try:
                icon_image = Image.open(icon_path)
                self._tk_window.iconphoto(False, ImageTk.PhotoImage(icon_image))
            except Exception as e:
                print(f"Error setting window icon: {str(e)}")
    
    def set_resizable(self, resizable=True):
        """
        Set whether the window is resizable.
        
        Args:
            resizable: Whether the window is resizable
        """
        self.resizable = resizable
        if self._tk_window:
            self._tk_window.resizable(resizable, resizable)
    
    def set_always_on_top(self, always_on_top=True):
        """
        Set whether the window is always on top.
        
        Args:
            always_on_top: Whether the window is always on top
        """
        self.always_on_top = always_on_top
        if self._tk_window:
            self._tk_window.attributes('-topmost', always_on_top)
    
    def set_alpha(self, alpha):
        """
        Set the window alpha transparency.
        
        Args:
            alpha: Alpha value (0.0 to 1.0)
        """
        self.alpha = max(0.0, min(1.0, alpha))
        if self._tk_window:
            try:
                self._tk_window.attributes('-alpha', self.alpha)
            except:
                pass  # Alpha might not be supported on all platforms
    
    def set_cursor(self, cursor):
        """
        Set the cursor for the window.
        
        Args:
            cursor: Cursor name
        """
        self.cursor = cursor
        if self._tk_window:
            self._tk_window.config(cursor=cursor)
    
    def center(self):
        """Center the window on the screen."""
        if self._tk_window:
            self._tk_window.update_idletasks()
            width = self._tk_window.winfo_width()
            height = self._tk_window.winfo_height()
            x = (self._tk_window.winfo_screenwidth() // 2) - (width // 2)
            y = (self._tk_window.winfo_screenheight() // 2) - (height // 2)
            self._tk_window.geometry(f"{width}x{height}+{x}+{y}")
            self.position = (x, y)
    
    def show(self):
        """Show the window."""
        if self._tk_window:
            self._tk_window.deiconify()
        super().show()
    
    def hide(self):
        """Hide the window."""
        if self._tk_window:
            self._tk_window.withdraw()
        super().hide()
    
    def minimize(self):
        """Minimize the window."""
        if self._tk_window:
            self._tk_window.iconify()
    
    def maximize(self):
        """Maximize the window."""
        if self._tk_window:
            self._tk_window.state('zoomed')
    
    def restore(self):
        """Restore the window from minimized or maximized state."""
        if self._tk_window:
            self._tk_window.state('normal')
    
    def run(self):
        """Run the window's main loop."""
        if self._tk_window:
            self._tk_window.mainloop()
    
    def close(self):
        """Close the window."""
        if self._tk_window:
            self._tk_window.destroy()


class Button(Widget):
    """Button widget."""
    
    def __init__(self, parent=None, text="Button", **kwargs):
        """
        Initialize the button.
        
        Args:
            parent: Parent widget
            text: Button text
            **kwargs: Additional button properties
        """
        super().__init__(parent, **kwargs)
        self.text = text
        self.image = kwargs.get('image', None)
        self.image_position = kwargs.get('image_position', 'left')
        self.toggle_button = kwargs.get('toggle', False)
        self.selected = False
        self.default = kwargs.get('default', False)
        self._tk_button = None
        
        # Create the underlying Tkinter button if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_button()
    
    def _create_tk_button(self):
        """Create the underlying Tkinter button."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the button
        self._tk_button = ttk.Button(
            self.parent._content_frame,
            text=self.text,
            command=self._on_click
        )
        
        # Set button position
        self._tk_button.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Set style if available
        if 'style' in self.style:
            self._tk_button.configure(style=self.style['style'])
    
    def _on_click(self):
        """Handle button click event."""
        # Toggle button state if it's a toggle button
        if self.toggle_button:
            self.selected = not self.selected
        
        # Trigger click event
        self.trigger_event(EventType.CLICK, {
            'text': self.text,
            'selected': self.selected
        })
    
    def set_text(self, text):
        """
        Set the button text.
        
        Args:
            text: New button text
        """
        self.text = text
        if self._tk_button:
            self._tk_button.configure(text=text)
    
    def get_text(self):
        """
        Get the button text.
        
        Returns:
            Button text
        """
        return self.text
    
    def set_image(self, image_path, position='left'):
        """
        Set the button image.
        
        Args:
            image_path: Path to the image file
            position: Image position ('left', 'right', 'top', 'bottom')
        """
        if not PIL_AVAILABLE:
            return
        
        try:
            # Load the image
            image = Image.open(image_path)
            
            # Resize if necessary
            if self.size and self.size != (0, 0):
                image = image.resize((self.size[0], self.size[1]), Image.LANCZOS)
            
            # Create PhotoImage
            self.image = ImageTk.PhotoImage(image)
            self.image_position = position
            
            # Update the button
            if self._tk_button:
                self._tk_button.configure(image=self.image, compound=position)
        except Exception as e:
            print(f"Error setting button image: {str(e)}")
    
    def set_toggle(self, toggle=True):
        """
        Set whether this is a toggle button.
        
        Args:
            toggle: Whether this is a toggle button
        """
        self.toggle_button = toggle
    
    def set_selected(self, selected=True):
        """
        Set the selected state of a toggle button.
        
        Args:
            selected: Whether the button is selected
        """
        if self.toggle_button:
            self.selected = selected
    
    def is_selected(self):
        """
        Check if a toggle button is selected.
        
        Returns:
            True if selected, False otherwise
        """
        return self.selected
    
    def set_default(self, default=True):
        """
        Set whether this is the default button.
        
        Args:
            default: Whether this is the default button
        """
        self.default = default


class Label(Widget):
    """Label widget for displaying text."""
    
    def __init__(self, parent=None, text="Label", **kwargs):
        """
        Initialize the label.
        
        Args:
            parent: Parent widget
            text: Label text
            **kwargs: Additional label properties
        """
        super().__init__(parent, **kwargs)
        self.text = text
        self.image = kwargs.get('image', None)
        self.image_position = kwargs.get('image_position', 'left')
        self.wrap_length = kwargs.get('wrap_length', 0)
        self.underline = kwargs.get('underline', -1)
        self.justify = kwargs.get('justify', Alignment.LEFT)
        self._tk_label = None
        
        # Create the underlying Tkinter label if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_label()
    
    def _create_tk_label(self):
        """Create the underlying Tkinter label."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the label
        self._tk_label = ttk.Label(
            self.parent._content_frame,
            text=self.text
        )
        
        # Set label position
        self._tk_label.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Set wrap length if specified
        if self.wrap_length > 0:
            self._tk_label.configure(wraplength=self.wrap_length)
        
        # Set underline if specified
        if self.underline >= 0:
            self._tk_label.configure(underline=self.underline)
        
        # Set justification
        if self.justify == Alignment.CENTER:
            justify_str = 'center'
        elif self.justify == Alignment.RIGHT:
            justify_str = 'right'
        else:
            justify_str = 'left'
        
        self._tk_label.configure(justify=justify_str)
        
        # Set style if available
        if 'style' in self.style:
            self._tk_label.configure(style=self.style['style'])
    
    def set_text(self, text):
        """
        Set the label text.
        
        Args:
            text: New label text
        """
        self.text = text
        if self._tk_label:
            self._tk_label.configure(text=text)
    
    def get_text(self):
        """
        Get the label text.
        
        Returns:
            Label text
        """
        return self.text
    
    def set_image(self, image_path, position='left'):
        """
        Set the label image.
        
        Args:
            image_path: Path to the image file
            position: Image position ('left', 'right', 'top', 'bottom')
        """
        if not PIL_AVAILABLE:
            return
        
        try:
            # Load the image
            image = Image.open(image_path)
            
            # Resize if necessary
            if self.size and self.size != (0, 0):
                image = image.resize((self.size[0], self.size[1]), Image.LANCZOS)
            
            # Create PhotoImage
            self.image = ImageTk.PhotoImage(image)
            self.image_position = position
            
            # Update the label
            if self._tk_label:
                self._tk_label.configure(image=self.image, compound=position)
        except Exception as e:
            print(f"Error setting label image: {str(e)}")
    
    def set_wrap_length(self, wrap_length):
        """
        Set the wrap length for the label.
        
        Args:
            wrap_length: Wrap length in pixels
        """
        self.wrap_length = wrap_length
        if self._tk_label:
            self._tk_label.configure(wraplength=wrap_length)
    
    def set_underline(self, index):
        """
        Set the character index to underline.
        
        Args:
            index: Character index to underline (-1 for none)
        """
        self.underline = index
        if self._tk_label:
            self._tk_label.configure(underline=index)
    
    def set_justify(self, justify):
        """
        Set the text justification.
        
        Args:
            justify: Justification (Alignment.LEFT, Alignment.CENTER, Alignment.RIGHT)
        """
        self.justify = justify
        
        if self._tk_label:
            if justify == Alignment.CENTER:
                justify_str = 'center'
            elif justify == Alignment.RIGHT:
                justify_str = 'right'
            else:
                justify_str = 'left'
            
            self._tk_label.configure(justify=justify_str)


class TextField(Widget):
    """Text field widget for single-line text input."""
    
    def __init__(self, parent=None, text="", **kwargs):
        """
        Initialize the text field.
        
        Args:
            parent: Parent widget
            text: Initial text
            **kwargs: Additional text field properties
        """
        super().__init__(parent, **kwargs)
        self.text = text
        self.placeholder = kwargs.get('placeholder', '')
        self.password = kwargs.get('password', False)
        self.read_only = kwargs.get('readonly', False)
        self.max_length = kwargs.get('maxlength', 0)
        self.justify = kwargs.get('justify', Alignment.LEFT)
        self._tk_entry = None
        
        # Create the underlying Tkinter entry if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_entry()
    
    def _create_tk_entry(self):
        """Create the underlying Tkinter entry."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the entry
        if self.password:
            self._tk_entry = ttk.Entry(
                self.parent._content_frame,
                show='*'
            )
        else:
            self._tk_entry = ttk.Entry(
                self.parent._content_frame
            )
        
        # Set entry position
        self._tk_entry.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Set initial text
        self._tk_entry.insert(0, self.text)
        
        # Set placeholder
        if self.placeholder:
            self._set_placeholder()
        
        # Bind events
        self._tk_entry.bind('<KeyRelease>', self._on_key_release)
        self._tk_entry.bind('<FocusIn>', self._on_focus_in)
        self._tk_entry.bind('<FocusOut>', self._on_focus_out)
        
        # Set read-only if needed
        if self.read_only:
            self._tk_entry.configure(state='readonly')
        
        # Set max length if specified
        if self.max_length > 0:
            self._tk_entry.configure(validate='key')
            self._tk_entry.configure(validatecommand=(
                self.parent._tk_window.register(self._validate_max_length), '%P'
            ))
        
        # Set justification
        if self.justify == Alignment.CENTER:
            justify_str = 'center'
        elif self.justify == Alignment.RIGHT:
            justify_str = 'right'
        else:
            justify_str = 'left'
        
        self._tk_entry.configure(justify=justify_str)
        
        # Set style if available
        if 'style' in self.style:
            self._tk_entry.configure(style=self.style['style'])
    
    def _set_placeholder(self):
        """Set placeholder text."""
        if not self._tk_entry or not self._tk_entry.get():
            self._tk_entry.insert(0, self.placeholder)
            self._tk_entry.configure(foreground='gray')
    
    def _clear_placeholder(self):
        """Clear placeholder text."""
        if self._tk_entry and self._tk_entry.get() == self.placeholder:
            self._tk_entry.delete(0, tk.END)
            self._tk_entry.configure(foreground='black')
    
    def _on_key_release(self, event):
        """Handle key release event."""
        text = self._tk_entry.get()
        self.text = text
        
        # Trigger key release event
        self.trigger_event(EventType.KEY_RELEASE, {
            'text': text,
            'key': event.keysym
        })
    
    def _on_focus_in(self, event):
        """Handle focus in event."""
        self._clear_placeholder()
        self.trigger_event(EventType.FOCUS_GAIN, {})
    
    def _on_focus_out(self, event):
        """Handle focus out event."""
        if not self._tk_entry.get():
            self._set_placeholder()
        
        self.trigger_event(EventType.FOCUS_LOSE, {})
    
    def _validate_max_length(self, new_text):
        """
        Validate text based on max length.
        
        Args:
            new_text: New text to validate
            
        Returns:
            True if valid, False otherwise
        """
        if self.max_length <= 0:
            return True
        
        return len(new_text) <= self.max_length
    
    def get_text(self):
        """
        Get the text field text.
        
        Returns:
            Text field text
        """
        if self._tk_entry and self._tk_entry.get() != self.placeholder:
            self.text = self._tk_entry.get()
        
        return self.text
    
    def set_text(self, text):
        """
        Set the text field text.
        
        Args:
            text: New text
        """
        self.text = text
        if self._tk_entry:
            self._tk_entry.delete(0, tk.END)
            self._tk_entry.insert(0, text)
    
    def set_placeholder(self, placeholder):
        """
        Set the placeholder text.
        
        Args:
            placeholder: New placeholder text
        """
        self.placeholder = placeholder
        if self._tk_entry and not self._tk_entry.get():
            self._set_placeholder()
    
    def set_password(self, password=True):
        """
        Set whether this is a password field.
        
        Args:
            password: Whether this is a password field
        """
        self.password = password
        if self._tk_entry:
            self._tk_entry.configure(show='*' if password else '')
    
    def set_readonly(self, readonly=True):
        """
        Set whether the text field is read-only.
        
        Args:
            readonly: Whether the text field is read-only
        """
        self.read_only = readonly
        if self._tk_entry:
            self._tk_entry.configure(state='readonly' if readonly else 'normal')
    
    def set_max_length(self, max_length):
        """
        Set the maximum length of the text.
        
        Args:
            max_length: Maximum length (0 for unlimited)
        """
        self.max_length = max_length
        
        if self._tk_entry:
            if max_length > 0:
                self._tk_entry.configure(validate='key')
                self._tk_entry.configure(validatecommand=(
                    self.parent._tk_window.register(self._validate_max_length), '%P'
                ))
            else:
                self._tk_entry.configure(validate='none')
    
    def set_justify(self, justify):
        """
        Set the text justification.
        
        Args:
            justify: Justification (Alignment.LEFT, Alignment.CENTER, Alignment.RIGHT)
        """
        self.justify = justify
        
        if self._tk_entry:
            if justify == Alignment.CENTER:
                justify_str = 'center'
            elif justify == Alignment.RIGHT:
                justify_str = 'right'
            else:
                justify_str = 'left'
            
            self._tk_entry.configure(justify=justify_str)
    
    def select_all(self):
        """Select all text in the field."""
        if self._tk_entry:
            self._tk_entry.selection_range(0, tk.END)
    
    def clear(self):
        """Clear the text field."""
        if self._tk_entry:
            self._tk_entry.delete(0, tk.END)
            self.text = ""


class TextArea(Widget):
    """Text area widget for multi-line text input."""
    
    def __init__(self, parent=None, text="", **kwargs):
        """
        Initialize the text area.
        
        Args:
            parent: Parent widget
            text: Initial text
            **kwargs: Additional text area properties
        """
        super().__init__(parent, **kwargs)
        self.text = text
        self.placeholder = kwargs.get('placeholder', '')
        self.read_only = kwargs.get('readonly', False)
        self.wrap = kwargs.get('wrap', 'word')
        self.scrollbars = kwargs.get('scrollbars', 'vertical')
        self.font = kwargs.get('font', None)
        self._tk_text = None
        self._tk_scrollbar_v = None
        self._tk_scrollbar_h = None
        
        # Create the underlying Tkinter text widget if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_text()
    
    def _create_tk_text(self):
        """Create the underlying Tkinter text widget."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create a frame for the text widget and scrollbars
        self._text_frame = ttk.Frame(self.parent._content_frame)
        self._text_frame.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Create scrollbars if needed
        if 'vertical' in self.scrollbars:
            self._tk_scrollbar_v = ttk.Scrollbar(
                self._text_frame,
                orient=tk.VERTICAL
            )
            self._tk_scrollbar_v.pack(side=tk.RIGHT, fill=tk.Y)
        
        if 'horizontal' in self.scrollbars:
            self._tk_scrollbar_h = ttk.Scrollbar(
                self._text_frame,
                orient=tk.HORIZONTAL
            )
            self._tk_scrollbar_h.pack(side=tk.BOTTOM, fill=tk.X)
        
        # Create the text widget
        xscrollcommand = None
        yscrollcommand = None
        
        if self._tk_scrollbar_v:
            yscrollcommand = self._tk_scrollbar_v.set
        
        if self._tk_scrollbar_h:
            xscrollcommand = self._tk_scrollbar_h.set
        
        self._tk_text = tk.Text(
            self._text_frame,
            wrap=self.wrap,
            xscrollcommand=xscrollcommand,
            yscrollcommand=yscrollcommand
        )
        
        # Set font if specified
        if self.font:
            self._tk_text.configure(font=self.font)
        
        # Set initial text
        self._tk_text.insert(tk.END, self.text)
        
        # Set placeholder
        if self.placeholder:
            self._set_placeholder()
        
        # Bind events
        self._tk_text.bind('<KeyRelease>', self._on_key_release)
        self._tk_text.bind('<FocusIn>', self._on_focus_in)
        self._tk_text.bind('<FocusOut>', self._on_focus_out)
        
        # Set read-only if needed
        if self.read_only:
            self._tk_text.configure(state='disabled')
        
        # Configure scrollbars
        if self._tk_scrollbar_v:
            self._tk_scrollbar_v.configure(command=self._tk_text.yview)
        
        if self._tk_scrollbar_h:
            self._tk_scrollbar_h.configure(command=self._tk_text.xview)
        
        # Pack the text widget
        self._tk_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
    
    def _set_placeholder(self):
        """Set placeholder text."""
        if not self._tk_text or not self._tk_text.get(1.0, tk.END).strip():
            self._tk_text.insert(tk.END, self.placeholder)
            self._tk_text.configure(foreground='gray')
    
    def _clear_placeholder(self):
        """Clear placeholder text."""
        if self._tk_text and self._tk_text.get(1.0, tk.END).strip() == self.placeholder:
            self._tk_text.delete(1.0, tk.END)
            self._tk_text.configure(foreground='black')
    
    def _on_key_release(self, event):
        """Handle key release event."""
        text = self._tk_text.get(1.0, tk.END)
        self.text = text
        
        # Trigger key release event
        self.trigger_event(EventType.KEY_RELEASE, {
            'text': text,
            'key': event.keysym
        })
    
    def _on_focus_in(self, event):
        """Handle focus in event."""
        self._clear_placeholder()
        self.trigger_event(EventType.FOCUS_GAIN, {})
    
    def _on_focus_out(self, event):
        """Handle focus out event."""
        if not self._tk_text.get(1.0, tk.END).strip():
            self._set_placeholder()
        
        self.trigger_event(EventType.FOCUS_LOSE, {})
    
    def get_text(self):
        """
        Get the text area text.
        
        Returns:
            Text area text
        """
        if self._tk_text and self._tk_text.get(1.0, tk.END).strip() != self.placeholder:
            self.text = self._tk_text.get(1.0, tk.END)
        
        return self.text
    
    def set_text(self, text):
        """
        Set the text area text.
        
        Args:
            text: New text
        """
        self.text = text
        if self._tk_text:
            self._tk_text.delete(1.0, tk.END)
            self._tk_text.insert(tk.END, text)
    
    def set_placeholder(self, placeholder):
        """
        Set the placeholder text.
        
        Args:
            placeholder: New placeholder text
        """
        self.placeholder = placeholder
        if self._tk_text and not self._tk_text.get(1.0, tk.END).strip():
            self._set_placeholder()
    
    def set_readonly(self, readonly=True):
        """
        Set whether the text area is read-only.
        
        Args:
            readonly: Whether the text area is read-only
        """
        self.read_only = readonly
        if self._tk_text:
            self._tk_text.configure(state='disabled' if readonly else 'normal')
    
    def set_wrap(self, wrap_mode):
        """
        Set the wrap mode.
        
        Args:
            wrap_mode: Wrap mode ('char', 'word', 'none')
        """
        self.wrap = wrap_mode
        if self._tk_text:
            self._tk_text.configure(wrap=wrap_mode)
    
    def set_font(self, font):
        """
        Set the text font.
        
        Args:
            font: Font specification
        """
        self.font = font
        if self._tk_text:
            self._tk_text.configure(font=font)
    
    def append_text(self, text):
        """
        Append text to the text area.
        
        Args:
            text: Text to append
        """
        if self._tk_text:
            self._tk_text.insert(tk.END, text)
            self.text = self._tk_text.get(1.0, tk.END)
    
    def clear(self):
        """Clear the text area."""
        if self._tk_text:
            self._tk_text.delete(1.0, tk.END)
            self.text = ""


class CheckBox(Widget):
    """Check box widget for boolean input."""
    
    def __init__(self, parent=None, text="CheckBox", checked=False, **kwargs):
        """
        Initialize the check box.
        
        Args:
            parent: Parent widget
            text: Check box text
            checked: Initial checked state
            **kwargs: Additional check box properties
        """
        super().__init__(parent, **kwargs)
        self.text = text
        self.checked = checked
        self._tk_checkbutton = None
        
        # Create the underlying Tkinter checkbutton if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_checkbutton()
    
    def _create_tk_checkbutton(self):
        """Create the underlying Tkinter checkbutton."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the checkbutton
        self._var = tk.BooleanVar(value=self.checked)
        self._tk_checkbutton = ttk.Checkbutton(
            self.parent._content_frame,
            text=self.text,
            variable=self._var,
            command=self._on_click
        )
        
        # Set checkbutton position
        self._tk_checkbutton.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Set style if available
        if 'style' in self.style:
            self._tk_checkbutton.configure(style=self.style['style'])
    
    def _on_click(self):
        """Handle check box click event."""
        self.checked = self._var.get()
        self.trigger_event(EventType.CLICK, {
            'text': self.text,
            'checked': self.checked
        })
    
    def get_text(self):
        """
        Get the check box text.
        
        Returns:
            Check box text
        """
        return self.text
    
    def set_text(self, text):
        """
        Set the check box text.
        
        Args:
            text: New check box text
        """
        self.text = text
        if self._tk_checkbutton:
            self._tk_checkbutton.configure(text=text)
    
    def is_checked(self):
        """
        Check if the check box is checked.
        
        Returns:
            True if checked, False otherwise
        """
        return self.checked
    
    def set_checked(self, checked=True):
        """
        Set the checked state of the check box.
        
        Args:
            checked: Whether the check box is checked
        """
        self.checked = checked
        if self._tk_checkbutton:
            self._var.set(checked)


class RadioButton(Widget):
    """Radio button widget for mutually exclusive selection."""
    
    def __init__(self, parent=None, text="RadioButton", group=None, selected=False, **kwargs):
        """
        Initialize the radio button.
        
        Args:
            parent: Parent widget
            text: Radio button text
            group: Radio button group
            selected: Initial selected state
            **kwargs: Additional radio button properties
        """
        super().__init__(parent, **kwargs)
        self.text = text
        self.group = group
        self.selected = selected
        self._tk_radiobutton = None
        
        # Create the underlying Tkinter radiobutton if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_radiobutton()
    
    def _create_tk_radiobutton(self):
        """Create the underlying Tkinter radiobutton."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the variable for the group if it doesn't exist
        if self.group and not hasattr(self.parent, f"_var_{self.group}"):
            setattr(self.parent, f"_var_{self.group}", tk.StringVar(value=self.id if self.selected else ""))
        
        # Create the radiobutton
        if self.group:
            var = getattr(self.parent, f"_var_{self.group}")
            self._tk_radiobutton = ttk.Radiobutton(
                self.parent._content_frame,
                text=self.text,
                value=self.id,
                variable=var,
                command=self._on_click
            )
        else:
            self._tk_radiobutton = ttk.Radiobutton(
                self.parent._content_frame,
                text=self.text,
                command=self._on_click
            )
        
        # Set radiobutton position
        self._tk_radiobutton.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Set style if available
        if 'style' in self.style:
            self._tk_radiobutton.configure(style=self.style['style'])
    
    def _on_click(self):
        """Handle radio button click event."""
        self.selected = True
        self.trigger_event(EventType.CLICK, {
            'text': self.text,
            'group': self.group,
            'selected': self.selected
        })
    
    def get_text(self):
        """
        Get the radio button text.
        
        Returns:
            Radio button text
        """
        return self.text
    
    def set_text(self, text):
        """
        Set the radio button text.
        
        Args:
            text: New radio button text
        """
        self.text = text
        if self._tk_radiobutton:
            self._tk_radiobutton.configure(text=text)
    
    def is_selected(self):
        """
        Check if the radio button is selected.
        
        Returns:
            True if selected, False otherwise
        """
        if self.group and hasattr(self.parent, f"_var_{self.group}"):
            var = getattr(self.parent, f"_var_{self.group}")
            self.selected = (var.get() == self.id)
        
        return self.selected
    
    def set_selected(self, selected=True):
        """
        Set the selected state of the radio button.
        
        Args:
            selected: Whether the radio button is selected
        """
        if self.group and hasattr(self.parent, f"_var_{self.group}"):
            var = getattr(self.parent, f"_var_{self.group}")
            if selected:
                var.set(self.id)
        
        self.selected = selected


class ComboBox(Widget):
    """Combo box widget for selecting from a list of options."""
    
    def __init__(self, parent=None, options=None, selected_index=0, **kwargs):
        """
        Initialize the combo box.
        
        Args:
            parent: Parent widget
            options: List of options
            selected_index: Index of the selected option
            **kwargs: Additional combo box properties
        """
        super().__init__(parent, **kwargs)
        self.options = options or []
        self.selected_index = selected_index
        self._tk_combobox = None
        
        # Create the underlying Tkinter combobox if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_combobox()
    
    def _create_tk_combobox(self):
        """Create the underlying Tkinter combobox."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the combobox
        self._tk_combobox = ttk.Combobox(
            self.parent._content_frame,
            values=self.options
        )
        
        # Set combobox position
        self._tk_combobox.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Set selected item
        if 0 <= self.selected_index < len(self.options):
            self._tk_combobox.current(self.selected_index)
        
        # Bind event
        self._tk_combobox.bind('<<ComboboxSelected>>', self._on_select)
        
        # Set style if available
        if 'style' in self.style:
            self._tk_combobox.configure(style=self.style['style'])
    
    def _on_select(self, event):
        """Handle combo box selection event."""
        self.selected_index = self._tk_combobox.current()
        selected_value = self._tk_combobox.get()
        self.trigger_event(EventType.CLICK, {
            'index': self.selected_index,
            'value': selected_value
        })
    
    def get_options(self):
        """
        Get the list of options.
        
        Returns:
            List of options
        """
        return self.options
    
    def set_options(self, options):
        """
        Set the list of options.
        
        Args:
            options: New list of options
        """
        self.options = options
        if self._tk_combobox:
            self._tk_combobox.configure(values=options)
            
            # Reset selection if it's out of bounds
            if self.selected_index >= len(options):
                self.selected_index = 0
                self._tk_combobox.current(0)
    
    def get_selected_index(self):
        """
        Get the index of the selected option.
        
        Returns:
            Selected index
        """
        if self._tk_combobox:
            self.selected_index = self._tk_combobox.current()
        
        return self.selected_index
    
    def set_selected_index(self, index):
        """
        Set the selected option by index.
        
        Args:
            index: Index of the option to select
        """
        if 0 <= index < len(self.options):
            self.selected_index = index
            if self._tk_combobox:
                self._tk_combobox.current(index)
    
    def get_selected_value(self):
        """
        Get the value of the selected option.
        
        Returns:
            Selected value
        """
        if self._tk_combobox:
            return self._tk_combobox.get()
        
        if 0 <= self.selected_index < len(self.options):
            return self.options[self.selected_index]
        
        return None
    
    def set_selected_value(self, value):
        """
        Set the selected option by value.
        
        Args:
            value: Value of the option to select
        """
        if value in self.options:
            index = self.options.index(value)
            self.set_selected_index(index)
    
    def add_option(self, option):
        """
        Add an option to the combo box.
        
        Args:
            option: Option to add
        """
        self.options.append(option)
        if self._tk_combobox:
            self._tk_combobox.configure(values=self.options)
    
    def remove_option(self, option):
        """
        Remove an option from the combo box.
        
        Args:
            option: Option to remove
        """
        if option in self.options:
            index = self.options.index(option)
            self.options.remove(option)
            
            if self._tk_combobox:
                self._tk_combobox.configure(values=self.options)
                
                # Adjust selected index if needed
                if self.selected_index == index:
                    if self.selected_index >= len(self.options):
                        self.selected_index = max(0, len(self.options) - 1)
                    self._tk_combobox.current(self.selected_index)


class ListBox(Widget):
    """List box widget for displaying a list of items."""
    
    def __init__(self, parent=None, items=None, selected_indices=None, **kwargs):
        """
        Initialize the list box.
        
        Args:
            parent: Parent widget
            items: List of items
            selected_indices: List of selected indices
            **kwargs: Additional list box properties
        """
        super().__init__(parent, **kwargs)
        self.items = items or []
        self.selected_indices = selected_indices or []
        self.selection_mode = kwargs.get('selection_mode', 'single')  # 'single', 'multiple', 'extended'
        self._tk_listbox = None
        
        # Create the underlying Tkinter listbox if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_listbox()
    
    def _create_tk_listbox(self):
        """Create the underlying Tkinter listbox."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the listbox
        self._tk_listbox = tk.Listbox(
            self.parent._content_frame,
            selectmode=self.selection_mode
        )
        
        # Set listbox position
        self._tk_listbox.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Add items
        for item in self.items:
            self._tk_listbox.insert(tk.END, item)
        
        # Select items
        for index in self.selected_indices:
            if 0 <= index < len(self.items):
                self._tk_listbox.selection_set(index)
        
        # Bind events
        self._tk_listbox.bind('<<ListboxSelect>>', self._on_select)
        
        # Set style if available
        if 'style' in self.style:
            self._tk_listbox.configure(style=self.style['style'])
    
    def _on_select(self, event):
        """Handle list box selection event."""
        # Get selected indices
        if self.selection_mode == 'single':
            selected = self._tk_listbox.curselection()
            if selected:
                self.selected_indices = [selected[0]]
            else:
                self.selected_indices = []
        else:
            self.selected_indices = list(self._tk_listbox.curselection())
        
        # Get selected items
        selected_items = []
        for index in self.selected_indices:
            if 0 <= index < len(self.items):
                selected_items.append(self.items[index])
        
        self.trigger_event(EventType.CLICK, {
            'indices': self.selected_indices,
            'items': selected_items
        })
    
    def get_items(self):
        """
        Get the list of items.
        
        Returns:
            List of items
        """
        return self.items
    
    def set_items(self, items):
        """
        Set the list of items.
        
        Args:
            items: New list of items
        """
        self.items = items
        if self._tk_listbox:
            # Clear current items
            self._tk_listbox.delete(0, tk.END)
            
            # Add new items
            for item in items:
                self._tk_listbox.insert(tk.END, item)
            
            # Reset selection
            self.selected_indices = []
    
    def add_item(self, item):
        """
        Add an item to the list box.
        
        Args:
            item: Item to add
        """
        self.items.append(item)
        if self._tk_listbox:
            self._tk_listbox.insert(tk.END, item)
    
    def remove_item(self, item):
        """
        Remove an item from the list box.
        
        Args:
            item: Item to remove
        """
        if item in self.items:
            index = self.items.index(item)
            self.items.remove(item)
            
            if self._tk_listbox:
                self._tk_listbox.delete(index)
                
                # Adjust selected indices
                new_selected_indices = []
                for selected_index in self.selected_indices:
                    if selected_index == index:
                        continue  # Removed item was selected
                    elif selected_index > index:
                        new_selected_indices.append(selected_index - 1)
                    else:
                        new_selected_indices.append(selected_index)
                
                self.selected_indices = new_selected_indices
                for index in self.selected_indices:
                    if 0 <= index < len(self.items):
                        self._tk_listbox.selection_set(index)
    
    def get_selected_indices(self):
        """
        Get the list of selected indices.
        
        Returns:
            List of selected indices
        """
        if self._tk_listbox:
            if self.selection_mode == 'single':
                selected = self._tk_listbox.curselection()
                if selected:
                    self.selected_indices = [selected[0]]
                else:
                    self.selected_indices = []
            else:
                self.selected_indices = list(self._tk_listbox.curselection())
        
        return self.selected_indices
    
    def set_selected_indices(self, indices):
        """
        Set the selected indices.
        
        Args:
            indices: List of indices to select
        """
        self.selected_indices = []
        if self._tk_listbox:
            # Clear current selection
            self._tk_listbox.selection_clear(0, tk.END)
            
            # Select new indices
            for index in indices:
                if 0 <= index < len(self.items):
                    self.selected_indices.append(index)
                    self._tk_listbox.selection_set(index)
    
    def get_selected_items(self):
        """
        Get the list of selected items.
        
        Returns:
            List of selected items
        """
        selected_items = []
        for index in self.selected_indices:
            if 0 <= index < len(self.items):
                selected_items.append(self.items[index])
        
        return selected_items
    
    def set_selection_mode(self, mode):
        """
        Set the selection mode.
        
        Args:
            mode: Selection mode ('single', 'multiple', 'extended')
        """
        self.selection_mode = mode
        if self._tk_listbox:
            self._tk_listbox.configure(selectmode=mode)
            
            # Reset selection if changing from multiple to single
            if mode == 'single' and self.selected_indices:
                self.set_selected_indices([self.selected_indices[0]])


class ProgressBar(Widget):
    """Progress bar widget for showing progress."""
    
    def __init__(self, parent=None, value=0, maximum=100, **kwargs):
        """
        Initialize the progress bar.
        
        Args:
            parent: Parent widget
            value: Current value
            maximum: Maximum value
            **kwargs: Additional progress bar properties
        """
        super().__init__(parent, **kwargs)
        self.value = value
        self.maximum = maximum
        self.orientation = kwargs.get('orientation', 'horizontal')  # 'horizontal', 'vertical'
        self._tk_progressbar = None
        
        # Create the underlying Tkinter progressbar if parent is a Window
        if parent and hasattr(parent, '_content_frame') and parent._content_frame:
            self._create_tk_progressbar()
    
    def _create_tk_progressbar(self):
        """Create the underlying Tkinter progressbar."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_content_frame'):
            return
        
        # Create the progressbar
        self._tk_progressbar = ttk.Progressbar(
            self.parent._content_frame,
            orient=self.orientation,
            maximum=self.maximum,
            value=self.value
        )
        
        # Set progressbar position
        self._tk_progressbar.place(
            x=self.position[0], 
            y=self.position[1],
            width=self.size[0], 
            height=self.size[1]
        )
        
        # Set style if available
        if 'style' in self.style:
            self._tk_progressbar.configure(style=self.style['style'])
    
    def get_value(self):
        """
        Get the current value.
        
        Returns:
            Current value
        """
        if self._tk_progressbar:
            self.value = self._tk_progressbar['value']
        
        return self.value
    
    def set_value(self, value):
        """
        Set the current value.
        
        Args:
            value: New value
        """
        self.value = max(0, min(self.maximum, value))
        if self._tk_progressbar:
            self._tk_progressbar['value'] = self.value
    
    def get_maximum(self):
        """
        Get the maximum value.
        
        Returns:
            Maximum value
        """
        return self.maximum
    
    def set_maximum(self, maximum):
        """
        Set the maximum value.
        
        Args:
            maximum: New maximum value
        """
        self.maximum = maximum
        if self._tk_progressbar:
            self._tk_progressbar['maximum'] = maximum
    
    def set_orientation(self, orientation):
        """
        Set the orientation.
        
        Args:
            orientation: Orientation ('horizontal', 'vertical')
        """
        self.orientation = orientation
        
        if self._tk_progressbar:
            # Update orientation
            self._tk_progressbar.configure(orient=orientation)
    
    def get_percentage(self):
        """
        Get the percentage of completion.
        
        Returns:
            Percentage (0-100)
        """
        if self.maximum <= 0:
            return 0
        
        return (self.value / self.maximum) * 100
    
    def set_percentage(self, percentage):
        """
        Set the value as a percentage of the maximum.
        
        Args:
            percentage: Percentage (0-100)
        """
        value = self.maximum * (percentage / 100)
        self.set_value(value)


class Menu(Widget):
    """Menu widget for creating menu bars and context menus."""
    
    def __init__(self, parent=None, **kwargs):
        """
        Initialize the menu.
        
        Args:
            parent: Parent widget
            **kwargs: Additional menu properties
        """
        super().__init__(parent, **kwargs)
        self.items = []
        self._tk_menu = None
        self._tearoff = kwargs.get('tearoff', 0)
        
        # Create the underlying Tkinter menu if parent is a Window
        if parent and hasattr(parent, '_tk_window'):
            self._create_tk_menu()
    
    def _create_tk_menu(self):
        """Create the underlying Tkinter menu."""
        if not TKINTER_AVAILABLE or not hasattr(self.parent, '_tk_window'):
            return
        
        # Create the menu
        self._tk_menu = tk.Menu(self.parent._tk_window, tearoff=self._tearoff)
        
        # Add items
        for item in self.items:
            self._add_item_to_tk_menu(item)
    
    def _add_item_to_tk_menu(self, item):
        """Add an item to the Tkinter menu."""
        if not self._tk_menu:
            return
        
        item_type = item.get('type', 'command')
        label = item.get('label', '')
        command = item.get('command', None)
        
        if item_type == 'command':
            self._tk_menu.add_command(
                label=label,
                command=lambda: self._on_item_click(item)
            )
        elif item_type == 'separator':
            self._tk_menu.add_separator()
        elif item_type == 'check':
            variable = tk.BooleanVar(value=item.get('checked', False))
            item['_variable'] = variable
            self._tk_menu.add_checkbutton(
                label=label,
                variable=variable,
                command=lambda: self._on_item_click(item)
            )
        elif item_type == 'radio':
            # This is simplified; a full implementation would handle radio groups
            variable = tk.StringVar(value=item.get('value', ''))
            item['_variable'] = variable
            self._tk_menu.add_radiobutton(
                label=label,
                variable=variable,
                value=item.get('value', ''),
                command=lambda: self._on_item_click(item)
            )
        elif item_type == 'cascade' and 'submenu' in item:
            submenu = Menu(self.parent)
            item['_submenu'] = submenu
            
            # Add submenu items
            for subitem in item.get('submenu', []):
                submenu.add_item(subitem)
            
            self._tk_menu.add_cascade(
                label=label,
                menu=submenu._tk_menu
            )
    
    def _on_item_click(self, item):
        """Handle menu item click event."""
        # Trigger click event
        self.trigger_event(EventType.CLICK, {
            'item': item
        })
        
        # Call the command if provided
        if 'command' in item and callable(item['command']):
            item['command']()
    
    def add_item(self, item):
        """
        Add an item to the menu.
        
        Args:
            item: Menu item dictionary
        """
        self.items.append(item)
        
        if self._tk_menu:
            self._add_item_to_tk_menu(item)
    
    def add_separator(self):
        """Add a separator to the menu."""
        self.add_item({'type': 'separator'})
    
    def add_command(self, label, command=None):
        """
        Add a command item to the menu.
        
        Args:
            label: Item label
            command: Command to execute
        """
        self.add_item({
            'type': 'command',
            'label': label,
            'command': command
        })
    
    def add_check(self, label, checked=False, command=None):
        """
        Add a check button item to the menu.
        
        Args:
            label: Item label
            checked: Initial checked state
            command: Command to execute
        """
        self.add_item({
            'type': 'check',
            'label': label,
            'checked': checked,
            'command': command
        })
    
    def add_radio(self, label, value='', command=None):
        """
        Add a radio button item to the menu.
        
        Args:
            label: Item label
            value: Radio button value
            command: Command to execute
        """
        self.add_item({
            'type': 'radio',
            'label': label,
            'value': value,
            'command': command
        })
    
    def add_submenu(self, label, submenu_items=None):
        """
        Add a submenu to the menu.
        
        Args:
            label: Submenu label
            submenu_items: List of submenu items
        """
        self.add_item({
            'type': 'cascade',
            'label': label,
            'submenu': submenu_items or []
        })
    
    def remove_item(self, index):
        """
        Remove an item from the menu.
        
        Args:
            index: Index of the item to remove
        """
        if 0 <= index < len(self.items):
            self.items.pop(index)
            
            # Recreate the menu
            if self._tk_menu:
                # Clear the menu
                self._tk_menu.delete(0, tk.END)
                
                # Add items again
                for item in self.items:
                    self._add_item_to_tk_menu(item)


class FileDialog:
    """File dialog class for file selection."""
    
    @staticmethod
    def open_file(parent=None, title="Open File", filetypes=None):
        """
        Show an open file dialog.
        
        Args:
            parent: Parent window
            title: Dialog title
            filetypes: List of file types
            
        Returns:
            Selected file path or None if canceled
        """
        if not TKINTER_AVAILABLE:
            return None
        
        # Default file types
        if filetypes is None:
            filetypes = [
                ("All Files", "*.*"),
                ("Text Files", "*.txt"),
                ("Python Files", "*.py"),
                ("Image Files", "*.png;*.jpg;*.jpeg;*.gif")
            ]
        
        if parent and hasattr(parent, '_tk_window'):
            return filedialog.askopenfilename(
                parent=parent._tk_window,
                title=title,
                filetypes=filetypes
            )
        else:
            return filedialog.askopenfilename(
                title=title,
                filetypes=filetypes
            )
    
    @staticmethod
    def save_file(parent=None, title="Save File", defaultextension=".txt", filetypes=None):
        """
        Show a save file dialog.
        
        Args:
            parent: Parent window
            title: Dialog title
            defaultextension: Default file extension
            filetypes: List of file types
            
        Returns:
            Selected file path or None if canceled
        """
        if not TKINTER_AVAILABLE:
            return None
        
        # Default file types
        if filetypes is None:
            filetypes = [
                ("All Files", "*.*"),
                ("Text Files", "*.txt"),
                ("Python Files", "*.py"),
                ("Image Files", "*.png;*.jpg;*.jpeg;*.gif")
            ]
        
        if parent and hasattr(parent, '_tk_window'):
            return filedialog.asksaveasfilename(
                parent=parent._tk_window,
                title=title,
                defaultextension=defaultextension,
                filetypes=filetypes
            )
        else:
            return filedialog.asksaveasfilename(
                title=title,
                defaultextension=defaultextension,
                filetypes=filetypes
            )
    
    @staticmethod
    def select_directory(parent=None, title="Select Directory"):
        """
        Show a directory selection dialog.
        
        Args:
            parent: Parent window
            title: Dialog title
            
        Returns:
            Selected directory path or None if canceled
        """
        if not TKINTER_AVAILABLE:
            return None
        
        if parent and hasattr(parent, '_tk_window'):
            return filedialog.askdirectory(
                parent=parent._tk_window,
                title=title
            )
        else:
            return filedialog.askdirectory(
                title=title
            )


class MessageBox:
    """Message box class for showing dialogs."""
    
    @staticmethod
    def show_info(parent=None, title="Information", message=""):
        """
        Show an information message box.
        
        Args:
            parent: Parent window
            title: Dialog title
            message: Message text
        """
        if not TKINTER_AVAILABLE:
            print(f"INFO: {title}\n{message}")
            return
        
        if parent and hasattr(parent, '_tk_window'):
            messagebox.showinfo(
                parent=parent._tk_window,
                title=title,
                message=message
            )
        else:
            messagebox.showinfo(
                title=title,
                message=message
            )
    
    @staticmethod
    def show_warning(parent=None, title="Warning", message=""):
        """
        Show a warning message box.
        
        Args:
            parent: Parent window
            title: Dialog title
            message: Message text
        """
        if not TKINTER_AVAILABLE:
            print(f"WARNING: {title}\n{message}")
            return
        
        if parent and hasattr(parent, '_tk_window'):
            messagebox.showwarning(
                parent=parent._tk_window,
                title=title,
                message=message
            )
        else:
            messagebox.showwarning(
                title=title,
                message=message
            )
    
    @staticmethod
    def show_error(parent=None, title="Error", message=""):
        """
        Show an error message box.
        
        Args:
            parent: Parent window
            title: Dialog title
            message: Message text
        """
        if not TKINTER_AVAILABLE:
            print(f"ERROR: {title}\n{message}")
            return
        
        if parent and hasattr(parent, '_tk_window'):
            messagebox.showerror(
                parent=parent._tk_window,
                title=title,
                message=message
            )
        else:
            messagebox.showerror(
                title=title,
                message=message
            )
    
    @staticmethod
    def ask_question(parent=None, title="Question", message="", default=None):
        """
        Show a question message box.
        
        Args:
            parent: Parent window
            title: Dialog title
            message: Message text
            default: Default answer ('yes', 'no', None)
            
        Returns:
            'yes', 'no', or None if canceled
        """
        if not TKINTER_AVAILABLE:
            answer = input(f"{title}\n{message}\n(y/n): ").lower()
            return 'yes' if answer.startswith('y') else 'no'
        
        if parent and hasattr(parent, '_tk_window'):
            result = messagebox.askyesno(
                parent=parent._tk_window,
                title=title,
                message=message
            )
            return 'yes' if result else 'no'
        else:
            result = messagebox.askyesno(
                title=title,
                message=message
            )
            return 'yes' if result else 'no'


class ColorDialog:
    """Color dialog class for color selection."""
    
    @staticmethod
    def choose_color(parent=None, title="Choose Color", initial_color=None):
        """
        Show a color selection dialog.
        
        Args:
            parent: Parent window
            title: Dialog title
            initial_color: Initial color (RGB tuple)
            
        Returns:
            Selected color (RGB tuple) or None if canceled
        """
        if not TKINTER_AVAILABLE:
            return None
        
        if parent and hasattr(parent, '_tk_window'):
            result = colorchooser.askcolor(
                parent=parent._tk_window,
                title=title,
                color=initial_color
            )
            return result[1] if result else None
        else:
            result = colorchooser.askcolor(
                title=title,
                color=initial_color
            )
            return result[1] if result else None


class LayoutManager:
    """Base class for layout managers."""
    
    def __init__(self, parent=None):
        """
        Initialize the layout manager.
        
        Args:
            parent: Parent widget
        """
        self.parent = parent
        self.widgets = []
    
    def add_widget(self, widget):
        """
        Add a widget to the layout.
        
        Args:
            widget: Widget to add
        """
        if widget not in self.widgets:
            self.widgets.append(widget)
    
    def remove_widget(self, widget):
        """
        Remove a widget from the layout.
        
        Args:
            widget: Widget to remove
        """
        if widget in self.widgets:
            self.widgets.remove(widget)
    
    def apply_layout(self):
        """Apply the layout to all widgets (abstract method)."""
        pass


class VerticalLayout(LayoutManager):
    """Vertical layout manager."""
    
    def __init__(self, parent=None, spacing=5):
        """
        Initialize the vertical layout.
        
        Args:
            parent: Parent widget
            spacing: Spacing between widgets
        """
        super().__init__(parent)
        self.spacing = spacing
    
    def apply_layout(self):
        """Apply the vertical layout to all widgets."""
        if not self.widgets:
            return
        
        # Get the parent bounds
        if hasattr(self.parent, 'get_bounds'):
            parent_x, parent_y, parent_width, parent_height = self.parent.get_bounds()
        else:
            parent_x, parent_y = 0, 0
            parent_width = parent_height = 1000  # Default large size
        
        # Calculate total height
        total_height = sum(widget.size[1] for widget in self.widgets)
        total_height += self.spacing * (len(self.widgets) - 1)
        
        # Calculate starting position (centered vertically)
        start_y = parent_y + (parent_height - total_height) // 2
        
        # Position widgets
        current_y = start_y
        for widget in self.widgets:
            # Center horizontally
            x = parent_x + (parent_width - widget.size[0]) // 2
            
            # Set widget position
            widget.set_bounds(x, current_y, widget.size[0], widget.size[1])
            
            # Move to next position
            current_y += widget.size[1] + self.spacing


class HorizontalLayout(LayoutManager):
    """Horizontal layout manager."""
    
    def __init__(self, parent=None, spacing=5):
        """
        Initialize the horizontal layout.
        
        Args:
            parent: Parent widget
            spacing: Spacing between widgets
        """
        super().__init__(parent)
        self.spacing = spacing
    
    def apply_layout(self):
        """Apply the horizontal layout to all widgets."""
        if not self.widgets:
            return
        
        # Get the parent bounds
        if hasattr(self.parent, 'get_bounds'):
            parent_x, parent_y, parent_width, parent_height = self.parent.get_bounds()
        else:
            parent_x, parent_y = 0, 0
            parent_width = parent_height = 1000  # Default large size
        
        # Calculate total width
        total_width = sum(widget.size[0] for widget in self.widgets)
        total_width += self.spacing * (len(self.widgets) - 1)
        
        # Calculate starting position (centered horizontally)
        start_x = parent_x + (parent_width - total_width) // 2
        
        # Position widgets
        current_x = start_x
        for widget in self.widgets:
            # Center vertically
            y = parent_y + (parent_height - widget.size[1]) // 2
            
            # Set widget position
            widget.set_bounds(current_x, y, widget.size[0], widget.size[1])
            
            # Move to next position
            current_x += widget.size[0] + self.spacing


class GridLayout(LayoutManager):
    """Grid layout manager."""
    
    def __init__(self, parent=None, rows=1, cols=1, h_spacing=5, v_spacing=5):
        """
        Initialize the grid layout.
        
        Args:
            parent: Parent widget
            rows: Number of rows
            cols: Number of columns
            h_spacing: Horizontal spacing between widgets
            v_spacing: Vertical spacing between widgets
        """
        super().__init__(parent)
        self.rows = rows
        self.cols = cols
        self.h_spacing = h_spacing
        self.v_spacing = v_spacing
        self.widget_positions = {}  # Map of widget to (row, col)
    
    def add_widget(self, widget, row=None, col=None):
        """
        Add a widget to the layout at a specific position.
        
        Args:
            widget: Widget to add
            row: Row position (None for auto-placement)
            col: Column position (None for auto-placement)
        """
        super().add_widget(widget)
        
        # Determine position
        if row is None or col is None:
            # Find first empty position
            for r in range(self.rows):
                for c in range(self.cols):
                    if (r, c) not in self.widget_positions.values():
                        row, col = r, c
                        break
                else:
                    continue
                break
        
        # Record position
        self.widget_positions[widget] = (row, col)
    
    def apply_layout(self):
        """Apply the grid layout to all widgets."""
        if not self.widgets:
            return
        
        # Get the parent bounds
        if hasattr(self.parent, 'get_bounds'):
            parent_x, parent_y, parent_width, parent_height = self.parent.get_bounds()
        else:
            parent_x, parent_y = 0, 0
            parent_width = parent_height = 1000  # Default large size
        
        # Calculate cell dimensions
        total_h_spacing = self.h_spacing * (self.cols - 1)
        total_v_spacing = self.v_spacing * (self.rows - 1)
        
        cell_width = (parent_width - total_h_spacing) // self.cols
        cell_height = (parent_height - total_v_spacing) // self.rows
        
        # Position widgets
        for widget in self.widgets:
            # Get widget position
            row, col = self.widget_positions.get(widget, (0, 0))
            
            # Calculate cell position
            x = parent_x + col * (cell_width + self.h_spacing)
            y = parent_y + row * (cell_height + self.v_spacing)
            
            # Center widget within cell
            w_x = x + (cell_width - widget.size[0]) // 2
            w_y = y + (cell_height - widget.size[1]) // 2
            
            # Set widget position
            widget.set_bounds(w_x, w_y, widget.size[0], widget.size[1])


class GUIApplication:
    """Main GUI application class."""
    
    def __init__(self):
        """Initialize the GUI application."""
        self.windows = []
        self.active_window = None
        self.styles = {}
        self.themes = {}
        self.current_theme = None
        
        # Initialize default styles
        self._init_default_styles()
        
        # Initialize default themes
        self._init_default_themes()
    
    def _init_default_styles(self):
        """Initialize default styles."""
        self.styles = {
            'button': {
                'font': ('Arial', 10),
                'padding': (8, 6),
                'background': '#f0f0f0',
                'foreground': '#000000',
                'border_width': 1,
                'border_color': '#cccccc'
            },
            'label': {
                'font': ('Arial', 10),
                'padding': (4, 2),
                'background': '#ffffff',
                'foreground': '#000000'
            },
            'textfield': {
                'font': ('Arial', 10),
                'padding': (4, 6),
                'background': '#ffffff',
                'foreground': '#000000',
                'border_width': 1,
                'border_color': '#cccccc'
            },
            'textarea': {
                'font': ('Arial', 10),
                'padding': (4, 6),
                'background': '#ffffff',
                'foreground': '#000000',
                'border_width': 1,
                'border_color': '#cccccc'
            },
            'checkbox': {
                'font': ('Arial', 10),
                'padding': (4, 2),
                'background': '#ffffff',
                'foreground': '#000000'
            },
            'radiobutton': {
                'font': ('Arial', 10),
                'padding': (4, 2),
                'background': '#ffffff',
                'foreground': '#000000'
            },
            'combobox': {
                'font': ('Arial', 10),
                'padding': (4, 6),
                'background': '#ffffff',
                'foreground': '#000000',
                'border_width': 1,
                'border_color': '#cccccc'
            },
            'listbox': {
                'font': ('Arial', 10),
                'background': '#ffffff',
                'foreground': '#000000',
                'border_width': 1,
                'border_color': '#cccccc'
            },
            'progressbar': {
                'height': 20,
                'background': '#f0f0f0',
                'foreground': '#4a90e2',
                'border_width': 1,
                'border_color': '#cccccc'
            }
        }
    
    def _init_default_themes(self):
        """Initialize default themes."""
        self.themes = {
            'default': {
                'background': '#f0f0f0',
                'foreground': '#000000',
                'selection': '#4a90e2',
                'selection_foreground': '#ffffff',
                'highlight': '#cccccc'
            },
            'dark': {
                'background': '#2d2d30',
                'foreground': '#f1f1f1',
                'selection': '#0078d7',
                'selection_foreground': '#ffffff',
                'highlight': '#3e3e42'
            },
            'light': {
                'background': '#ffffff',
                'foreground': '#000000',
                'selection': '#0078d7',
                'selection_foreground': '#ffffff',
                'highlight': '#e1e1e1'
            },
            'blue': {
                'background': '#f0f8ff',
                'foreground': '#000000',
                'selection': '#1e90ff',
                'selection_foreground': '#ffffff',
                'highlight': '#b0d4f1'
            }
        }
        
        # Set the default theme
        self.current_theme = self.themes['default']
    
    def set_theme(self, theme_name):
        """
        Set the current theme.
        
        Args:
            theme_name: Name of the theme
        """
        if theme_name in self.themes:
            self.current_theme = self.themes[theme_name]
    
    def create_window(self, title="Window", width=800, height=600, **kwargs):
        """
        Create a new window.
        
        Args:
            title: Window title
            width: Window width
            height: Window height
            **kwargs: Additional window properties
            
        Returns:
            Window instance
        """
        window = Window(title, width, height, **kwargs)
        self.windows.append(window)
        self.active_window = window
        return window
    
    def create_button(self, parent=None, text="Button", **kwargs):
        """
        Create a new button.
        
        Args:
            parent: Parent widget
            text: Button text
            **kwargs: Additional button properties
            
        Returns:
            Button instance
        """
        if parent is None:
            parent = self.active_window
        
        return Button(parent, text, **kwargs)
    
    def create_label(self, parent=None, text="Label", **kwargs):
        """
        Create a new label.
        
        Args:
            parent: Parent widget
            text: Label text
            **kwargs: Additional label properties
            
        Returns:
            Label instance
        """
        if parent is None:
            parent = self.active_window
        
        return Label(parent, text, **kwargs)
    
    def create_text_field(self, parent=None, text="", **kwargs):
        """
        Create a new text field.
        
        Args:
            parent: Parent widget
            text: Initial text
            **kwargs: Additional text field properties
            
        Returns:
            TextField instance
        """
        if parent is None:
            parent = self.active_window
        
        return TextField(parent, text, **kwargs)
    
    def create_text_area(self, parent=None, text="", **kwargs):
        """
        Create a new text area.
        
        Args:
            parent: Parent widget
            text: Initial text
            **kwargs: Additional text area properties
            
        Returns:
            TextArea instance
        """
        if parent is None:
            parent = self.active_window
        
        return TextArea(parent, text, **kwargs)
    
    def create_check_box(self, parent=None, text="CheckBox", checked=False, **kwargs):
        """
        Create a new check box.
        
        Args:
            parent: Parent widget
            text: Check box text
            checked: Initial checked state
            **kwargs: Additional check box properties
            
        Returns:
            CheckBox instance
        """
        if parent is None:
            parent = self.active_window
        
        return CheckBox(parent, text, checked, **kwargs)
    
    def create_radio_button(self, parent=None, text="RadioButton", group=None, selected=False, **kwargs):
        """
        Create a new radio button.
        
        Args:
            parent: Parent widget
            text: Radio button text
            group: Radio button group
            selected: Initial selected state
            **kwargs: Additional radio button properties
            
        Returns:
            RadioButton instance
        """
        if parent is None:
            parent = self.active_window
        
        return RadioButton(parent, text, group, selected, **kwargs)
    
    def create_combo_box(self, parent=None, options=None, selected_index=0, **kwargs):
        """
        Create a new combo box.
        
        Args:
            parent: Parent widget
            options: List of options
            selected_index: Index of the selected option
            **kwargs: Additional combo box properties
            
        Returns:
            ComboBox instance
        """
        if parent is None:
            parent = self.active_window
        
        return ComboBox(parent, options, selected_index, **kwargs)
    
    def create_list_box(self, parent=None, items=None, selected_indices=None, **kwargs):
        """
        Create a new list box.
        
        Args:
            parent: Parent widget
            items: List of items
            selected_indices: List of selected indices
            **kwargs: Additional list box properties
            
        Returns:
            ListBox instance
        """
        if parent is None:
            parent = self.active_window
        
        return ListBox(parent, items, selected_indices, **kwargs)
    
    def create_progress_bar(self, parent=None, value=0, maximum=100, **kwargs):
        """
        Create a new progress bar.
        
        Args:
            parent: Parent widget
            value: Current value
            maximum: Maximum value
            **kwargs: Additional progress bar properties
            
        Returns:
            ProgressBar instance
        """
        if parent is None:
            parent = self.active_window
        
        return ProgressBar(parent, value, maximum, **kwargs)
    
    def create_menu(self, parent=None, **kwargs):
        """
        Create a new menu.
        
        Args:
            parent: Parent widget
            **kwargs: Additional menu properties
            
        Returns:
            Menu instance
        """
        if parent is None:
            parent = self.active_window
        
        return Menu(parent, **kwargs)
    
    def run(self):
        """Run the GUI application."""
        if self.active_window:
            self.active_window.run()


def create_sample_application():
    """Create a sample GUI application."""
    print("GUI Application Framework")
    print("========================")
    print("Checking dependencies:")
    print(f"Tkinter available: {'Yes' if TKINTER_AVAILABLE else 'No'}")
    print(f"PIL available: {'Yes' if PIL_AVAILABLE else 'No'}")
    
    if not TKINTER_AVAILABLE:
        print("This library requires Tkinter.")
        return
    
    # Create the application
    app = GUIApplication()
    
    # Set the theme
    app.set_theme('light')
    
    # Create a window
    window = app.create_window("Sample GUI Application", 800, 600)
    window.center()
    
    # Add event handler for window close
    def on_close(widget, event_type, event_data):
        print("Application closing")
    
    window.add_event_handler(EventType.CLOSE, on_close)
    
    # Create a menu bar
    menu_bar = app.create_menu(window)
    menu_bar.add_command("File", lambda: print("File menu clicked"))
    menu_bar.add_command("Edit", lambda: print("Edit menu clicked"))
    menu_bar.add_command("View", lambda: print("View menu clicked"))
    menu_bar.add_command("Help", lambda: print("Help menu clicked"))
    
    # Set the menu bar
    window.menu_bar = menu_bar
    
    # Create a label
    label = app.create_label(window, "Sample GUI Application", position=(10, 10), size=(780, 30))
    
    # Create a text field
    text_field = app.create_text_field(window, "Enter text here", position=(10, 50), size=(300, 30))
    
    # Create a button
    button = app.create_button(window, "Click Me", position=(10, 90), size=(100, 30))
    
    # Add event handler for button click
    def on_button_click(widget, event_type, event_data):
        text = text_field.get_text()
        label.set_text(f"Button clicked! Text: {text}")
        print(f"Button clicked! Text: {text}")
    
    button.add_event_handler(EventType.CLICK, on_button_click)
    
    # Create a check box
    check_box = app.create_check_box(window, "Check me", position=(120, 90), size=(100, 30))
    
    # Create radio buttons
    radio1 = app.create_radio_button(window, "Option 1", group="options", position=(10, 130), size=(100, 30), selected=True)
    radio2 = app.create_radio_button(window, "Option 2", group="options", position=(120, 130), size=(100, 30))
    
    # Create a combo box
    combo = app.create_combo_box(window, options=["Option 1", "Option 2", "Option 3"], position=(10, 170), size=(200, 30))
    
    # Create a list box
    list_box = app.create_list_box(window, items=["Item 1", "Item 2", "Item 3", "Item 4", "Item 5"], position=(10, 210), size=(200, 150))
    
    # Create a text area
    text_area = app.create_text_area(window, "This is a text area.\nIt can contain multiple lines of text.", position=(220, 50), size=(570, 310))
    
    # Create a progress bar
    progress = app.create_progress_bar(window, value=30, maximum=100, position=(10, 370), size=(780, 30))
    
    # Create a horizontal layout for buttons
    h_layout = HorizontalLayout()
    
    # Create buttons for the layout
    ok_button = app.create_button(window, "OK", position=(0, 0), size=(100, 30))
    cancel_button = app.create_button(window, "Cancel", position=(0, 0), size=(100, 30))
    apply_button = app.create_button(window, "Apply", position=(0, 0), size=(100, 30))
    
    # Add buttons to layout
    h_layout.add_widget(ok_button)
    h_layout.add_widget(cancel_button)
    h_layout.add_widget(apply_button)
    
    # Create a container widget for the layout
    container = Widget(window, position=(10, 410), size=(780, 50))
    
    # Set the layout parent to the container
    h_layout.parent = container
    
    # Apply the layout
    h_layout.apply_layout()
    
    # Add event handlers for the buttons
    def on_ok(widget, event_type, event_data):
        print("OK button clicked")
        MessageBox.show_info(window, "Information", "OK button clicked!")
    
    def on_cancel(widget, event_type, event_data):
        print("Cancel button clicked")
        MessageBox.show_info(window, "Information", "Cancel button clicked!")
    
    def on_apply(widget, event_type, event_data):
        print("Apply button clicked")
        
        # Update progress bar
        current_value = progress.get_value()
        new_value = min(current_value + 10, 100)
        progress.set_value(new_value)
        
        MessageBox.show_info(window, "Information", f"Apply button clicked! Progress: {progress.get_percentage():.1f}%")
    
    ok_button.add_event_handler(EventType.CLICK, on_ok)
    cancel_button.add_event_handler(EventType.CLICK, on_cancel)
    apply_button.add_event_handler(EventType.CLICK, on_apply)
    
    # Add event handler for combo box selection
    def on_combo_select(widget, event_type, event_data):
        print(f"Combo box selected: {event_data.get('value', '')}")
    
    combo.add_event_handler(EventType.CLICK, on_combo_select)
    
    # Add event handler for list box selection
    def on_list_select(widget, event_type, event_data):
        selected_items = event_data.get('items', [])
        print(f"List box selected: {selected_items}")
    
    list_box.add_event_handler(EventType.CLICK, on_list_select)
    
    # Create a timer to update the progress bar
    def update_progress():
        current_value = progress.get_value()
        new_value = (current_value + 1) % 101
        progress.set_value(new_value)
        
        if hasattr(window, '_tk_window'):
            window._tk_window.after(100, update_progress)
    
    # Start the timer
    if hasattr(window, '_tk_window'):
        window._tk_window.after(1000, update_progress)
    
    # Create a menu item for changing themes
    def change_theme_dark(widget, event_type, event_data):
        app.set_theme('dark')
        print("Changed to dark theme")
    
    def change_theme_light(widget, event_type, event_data):
        app.set_theme('light')
        print("Changed to light theme")
    
    # Create a context menu
    context_menu = app.create_menu(window)
    context_menu.add_command("Change to Dark Theme", change_theme_dark)
    context_menu.add_command("Change to Light Theme", change_theme_light)
    
    # Add event handler for right-click on the window
    def on_right_click(widget, event_type, event_data):
        if hasattr(window, '_tk_window'):
            # Show the context menu
            if hasattr(window, '_tk_window'):
                x = event_data.get('x', 0)
                y = event_data.get('y', 0)
                context_menu._tk_menu.post(x, y)
    
    # Add event handler for right-click (this is simplified)
    # In a real implementation, you would bind this to the window's right-click event
    
    # Run the application
    print("Running the GUI application...")
    app.run()


def main():
    """Main function to demonstrate the GUI framework."""
    print("GUI Application Framework")
    print("========================")
    
    # Create and run the sample application
    create_sample_application()
    
    return True


if __name__ == "__main__":
    main()
<｜fim▁hole｜>