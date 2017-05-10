/**
	ControllerInventoryBar

	Displays inventory slots and extra information.

	@authors Zapper, Clonkonaut
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Original code by Zapper and Clonkonaut

/*
	inventory_slot contains an array of proplists with the following attributes:
		ID: submenu ID. Unique in combination with the target == this
		obj: last object that was shown here
		hand: bool, whether select with a hand
		quick: bool, whether this is the quick switch slot
*/

// HUD margin and size in tenths of em.
static const GUI_Controller_InventoryBar_IconMarginScreenTop = 5;
static const GUI_Controller_InventoryBar_IconSize = 20;
static const GUI_Controller_InventoryBar_IconMargin = 5;

local inventory_slots;
local inventory_gui_menu;
local inventory_gui_id;

/* GUI creation */

// For custom HUD graphics overload the following function as deemed fit.

func AssembleInventoryButton(int max_slots, int slot_number, proplist slot_info)
{
	// The gui already exists, only update it with a new submenu
	var pos = CalculateButtonPosition(slot_number, max_slots);
	var content =
	{
		Target = this,
		slot_number =
		{
			Priority = 3, // Make sure the slot number is drawn above the icon.
			Style = GUI_TextTop,
			Text = Format("%2d", slot_info.slot + 1)
		},
		quick_switch = // Shows quick switch control key if this is the quick switch slot
		{
			Priority = 3,
			Style = GUI_NoCrop | GUI_TextHCenter | GUI_TextBottom,
			Left = "-50%",
			Right = "150%",
			Top = Format(" %s%s", "20%", ToEmString(-2)),
			Bottom = "20%",
			Text = { Std = "", Quick = Format("<c dddd00>[%s]</c>", GetPlayerControlAssignment(GetOwner(), CON_QuickSwitch, true)), Selected = "" }
		},
		Style = GUI_NoCrop,
		ID = slot_info.ID,
		Symbol = {Std = Icon_Menu_Circle, Quick = Icon_Menu_Circle, Selected = Icon_Menu_CircleHighlight},
		count =
		{
			ID = 1000 + slot_info.ID,
			Style = GUI_TextRight | GUI_TextBottom,
			Text = nil,
			Priority = 2
		},
		// Prepare (invisible) extra-slot display circle.
		extra_slot =
		{
			Top = ToEmString(GUI_Controller_InventoryBar_IconSize),
			Bottom = ToEmString(GUI_Controller_InventoryBar_IconSize + GUI_Controller_InventoryBar_IconSize/2),
			Style = GUI_TextLeft,
			Text = nil,
			symbol =// used to display an infinity sign if necessary (Icon_Number)
			{
				Right = ToEmString(GUI_Controller_InventoryBar_IconSize/2),
				GraphicsName = "Inf",
			},
			circle =// shows the item in the extra slot
			{
				Left = ToEmString(GUI_Controller_InventoryBar_IconSize/2),
				Symbol = nil,
				symbol = {}
			}
		},
		overlay = // Custom inventory overlays can be shown here.
		{
			ID = 2000 + slot_info.ID
		}
	};
	
	return AddProperties(content, pos);
}

/* Creation / Destruction */

private func Construction()
{
	inventory_slots = [];

	inventory_gui_menu =
	{
		Target = this,
		Player = NO_OWNER, // will be shown once a gui update occurs
		Style = GUI_Multiple | GUI_IgnoreMouse | GUI_NoCrop
	};
	inventory_gui_id = GuiOpen(inventory_gui_menu);

	return _inherited(...);
}

private func Destruction()
{
	GuiClose(inventory_gui_id);

	_inherited(...);
}

/* Callbacks */

public func OnCrewDisabled(object clonk)
{
	ScheduleUpdateInventory();

	return _inherited(clonk, ...);
}

public func OnCrewEnabled(object clonk)
{
	ScheduleUpdateInventory();

	return _inherited(clonk, ...);
}

public func OnCrewSelection(object clonk, bool deselect)
{
	ScheduleUpdateInventory();

	return _inherited(clonk, deselect, ...);
}

// call from HUDAdapter (Clonk)
public func OnSlotObjectChanged(int slot)
{
	// refresh inventory
	ScheduleUpdateInventory();

	return _inherited(slot, ...);
}

// Updates the Inventory in 1 frame
public func ScheduleUpdateInventory()
{
	if (!GetEffect("UpdateInventory", this))
		AddEffect("UpdateInventory", this, 1, 1, this);
}

private func FxUpdateInventoryTimer()
{
	UpdateInventory();
	return FX_Execute_Kill;
}

/* Display */

private func UpdateInventory()
{
	// only display if we have a clonk and it's not disabled
	var clonk = GetCursor(GetOwner());
	if(!clonk || !clonk->GetCrewEnabled())
	{
		if (inventory_gui_menu.Player != NO_OWNER)
		{
			inventory_gui_menu.Player = NO_OWNER;
			GuiUpdate(inventory_gui_menu, inventory_gui_id);
		}

		return;
	}

	// Make sure inventory is visible
	if (inventory_gui_menu.Player != GetOwner())
	{
		inventory_gui_menu.Player = GetOwner();
		GuiUpdate(inventory_gui_menu, inventory_gui_id);
	}

	UpdateInventoryButtons(clonk);

	// update inventory-slots
	var hand_item_pos = clonk->~GetHandItemPos(0);
	var quick_switch_slot = clonk->~GetQuickSwitchSlot();

	for (var slot_info in inventory_slots)
	{
		var item = clonk->GetItem(slot_info.slot);
		// Enable objects to provide a custom overlay for the icon slot.
		// This could e.g. be used by special scenarios or third-party mods.
		var custom_overlay = nil;
		// For stacked objects, we will have multiple virtual objects in one slot.
		var stack_count = nil;
		if (item)
		{
			stack_count = item->~GetStackCount();
			custom_overlay = item->~GetInventoryIconOverlay();
		}
		var needs_selection = hand_item_pos == slot_info.slot;
		var needs_quick_switch = quick_switch_slot == slot_info.slot;
		var has_extra_slot = item && item->~HasExtraSlot();
		if ((!!item == slot_info.empty) || (item != slot_info.obj) || (needs_selection != slot_info.hand) || (needs_quick_switch != slot_info.quick) || (stack_count != slot_info.last_count) || has_extra_slot || slot_info.had_custom_overlay || custom_overlay)
		{
			// Hide or show extra-slot display?
			var extra_slot_player = NO_OWNER;
			var extra_symbol = nil;
			var contents = nil;
			var extra_slot_background_symbol = nil;
			if (has_extra_slot)
			{
				// Show!
				contents = item->Contents(0);
				if (contents)
					extra_symbol = contents->GetID();
				extra_slot_player = GetOwner();
				extra_slot_background_symbol = Icon_Menu_Circle;
				// And attach tracker..
				var i = 0, e = nil;
				var found = false;
				while (e = GetEffect("ExtraSlotUpdater", item, i++))
				{
					if (e.CommandTarget != this) continue;
					found = true;
					break;
				}
				if (!found) AddEffect("ExtraSlotUpdater", item, 1, 30 + Random(60), this);
			}
			// What to display in the extra slot?
			var extra_text = nil, number_symbol = nil;
			if (extra_symbol && contents->~GetStackCount())
			{
				if (contents->IsInfiniteStackCount())
					number_symbol = Icon_Number;
				else extra_text = Format("%dx", contents->GetStackCount());
			}
			
			// Close a possible lingering custom overlay for that slot.
			var custom_overlay_id = 2000 + slot_info.ID;
			GuiClose(inventory_gui_id, custom_overlay_id, nil);
			
			// Compose the update!
			var update =
			{
				slot = { Symbol = item },
				extra_slot =
				{
					Player = extra_slot_player,
					Text = extra_text,
					symbol =
					{
						Symbol = number_symbol
					},
					circle =
					{
						Symbol = extra_slot_background_symbol,
						symbol = { Symbol = extra_symbol }
					}
				},
				count = 
				{
					Text = ""
				}
			};
			
			if (item)
			{
				if (stack_count > 1 && !item->~IsInfiniteStackCount())
				{
					update.count.Text = Format("%dx", stack_count);
					slot_info.last_count = stack_count;
				}
			}
			else
			{
				slot_info.last_count = nil;
			}
			
			if (custom_overlay)
			{
				update.overlay = custom_overlay;
				update.overlay.ID = custom_overlay_id;
				slot_info.had_custom_overlay = true;
			}
			else
			{
				slot_info.had_custom_overlay = false;
			}
			
			GuiUpdate(update, inventory_gui_id, slot_info.ID, this);

			var tag = "Std";
			if (needs_quick_switch) tag = "Quick";
			if (needs_selection) tag = "Selected";
			GuiUpdateTag(tag, inventory_gui_id, slot_info.ID, this);

			slot_info.hand = needs_selection;
			slot_info.quick = needs_quick_switch;
			slot_info.obj = item;
			slot_info.empty = !item;
		}
	}
}

// Sets the inventory size to the currently selected clonk
private func UpdateInventoryButtons(object clonk)
{
	var max_contents_count = clonk.MaxContentsCount;

	var old_count = GetLength(inventory_slots);

	// need to create more inventory buttons?
	while (max_contents_count > GetLength(inventory_slots))
		CreateNewInventoryButton(max_contents_count);

	// need to remove some inventory buttons?
	while (max_contents_count < GetLength(inventory_slots))
	{
		var slot_info = inventory_slots[-1];
		GuiClose(inventory_gui_id, slot_info.ID, this);
		SetLength(inventory_slots, GetLength(inventory_slots)-1);
	}

	// modifications occured? Adjust position of old slots
	if (old_count != max_contents_count)
	{
		for (var i = 0; i < Min(old_count, max_contents_count); ++i)
		{
			var slot_info = inventory_slots[i];
			var update = CalculateButtonPosition(i, max_contents_count);
			GuiUpdate(update, inventory_gui_id, slot_info.ID, this);
		}
	}
}

// Insert an inventory slot into the inventory-bar
private func CreateNewInventoryButton(int max_slots)
{
	var slot_number = GetLength(inventory_slots);
	var slot_info =
	{
		slot = slot_number,
		ID = slot_number + 1,
		hand = false,
		quick = false,
		obj = nil,
		empty = true
	};
	PushBack(inventory_slots, slot_info);

	var slot = AssembleInventoryButton(max_slots, slot_number, slot_info);

	GuiUpdate({_new_icon = slot}, inventory_gui_id);
}

// Calculates the position of a specific button and returns a proplist.
private func CalculateButtonPosition(int slot_number, int max_slots)
{
	return GetGridPosition(InventoryBarLayout(), 0, slot_number, 1, max_slots);
}

private func FxExtraSlotUpdaterTimer(object target, proplist effect)
{
	if (!this) return FX_Execute_Kill;
	if (!target) return FX_Execute_Kill;
	if (target->Contained() != GetCursor(GetOwner())) return FX_Execute_Kill;
	return FX_OK;
}

private func FxExtraSlotUpdaterUpdate(object target, proplist effect)
{
	if (this) ScheduleUpdateInventory();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Configuration functions

static const GUI_AlignLeft = -1;
static const GUI_AlignCenter = 0;
static const GUI_AlignRight = +1;

static const GUI_AlignTop = -1;
static const GUI_AlignBottom = +1;

static const GUI_Layout = new Global {
	AlignX = GUI_AlignLeft,
	AlignY = GUI_AlignTop,
	MarginX = 0,
	MarginY = 0,
	Width = 0,
	Height = 0,
	Dimenstion = Global.ToPercentString,
};

public func InventoryBarLayout()
{
	return {
		Prototype = GUI_Layout,
		AlignX = GUI_AlignCenter,
		AlignY = GUI_AlignTop,
		MarginX = GUI_Controller_InventoryBar_IconMargin,
		MarginY = GUI_Controller_InventoryBar_IconMarginScreenTop,
		Width = GUI_Controller_InventoryBar_IconSize,
		Height = GUI_Controller_InventoryBar_IconSize,
		Dimension = Global.ToEmString,
	};
}

public func CheckLayout(proplist layout)
{
	var errors = [];
	if (layout.Width == 0)
	{
		errors = PushBack(errors, "property 'Width' must not be 0");
	}
	if (layout.Height == 0)
	{
		errors = PushBack(errors, "property 'Height' must not be 0");
	}
	if (layout.Dimension != Global.ToEmString && layout.Dimension != Global.ToPercentString)
	{
		errors = PushBack(errors, Format("property 'Dimension' must be Global.ToEmString, or Global.ToPerccentString, but it is %v", layout.Dimension));
	}
	
	if (GetLength(errors) > 0)
	{
		var message = "Error in layout";
		for (var error in errors)
		{
			message = Format("%s, %s", message, error);
		}
		FatalError(message);
	}
}


public func GetElementPosition(proplist layout)
{
	CheckLayout(layout);

	var element_width = layout.Width + 2 * layout.MarginX;
	var element_height = layout.Height + 2 * layout.MarginY;

	// determine alignment on x axis
	var align_x;
	var offset_x;
	if (layout.AlignX == GUI_AlignLeft)
	{
		align_x = "0%";
		offset_x = 0;
	}
	else if (layout.AlignX == GUI_AlignCenter)
	{
		align_x = "50%";
		offset_x = -element_width / 2;
	}
	else if (layout.AlignX == GUI_AlignRight)
	{
		align_x = "100%";
		offset_x = -element_width;
	}

	// determine alignment on y axis
	var align_y;
	var offset_y;
	if (layout.AlignY == GUI_AlignTop)
	{
		align_y = "0%";
		offset_y = 0;
	}
	else if (layout.AlignY == GUI_AlignCenter)
	{
		align_y = "50%";
		offset_y = -element_height / 2;
	}
	else if (layout.AlignY == GUI_AlignBottom)
	{
		align_y = "100%";
		offset_y = -element_height;
	}

	// determine actual dimensions

	var element_x = offset_x + layout.MarginX;
	var element_y = offset_y + layout.MarginY;

	return
	{
		Left =   Format("%s%s", align_x, Call(layout.Dimension, element_x)),
		Top =    Format("%s%s", align_y, Call(layout.Dimension, element_y)),
		Right =  Format("%s%s", align_x, Call(layout.Dimension, element_x + layout.Width)),
		Bottom = Format("%s%s", align_y, Call(layout.Dimension, element_y + layout.Height))
	};
}


public func GetGridPosition(proplist layout, int row, int column, int grid_rows, int grid_columns)
{
	// determine position of the grid
	var grid_width = cell_width * grid_columns;
	var grid_height = cell_height * grid_rows;
	
	var grid_layout = {
		Prototype = layout,
		Width = grid_width,
		Height = grid_height,
	};

	var grid_position = GetElementPosition(grid_layout);
	
	// determine position of the cell in the grid
	var cell_width = layout.Width + 2 * layout.MarginX;
	var cell_height = layout.Height + 2 * layout.MarginY;

	var cell_pos_x = layout.MarginX + column * cell_width;
	var cell_pos_y = layout.MarginY + row * cell_height;
	
	return
	{
		Left =   Format("%s%s", grid_position.Left, Call(layout.Dimension, cell_pos_x)),
		Top =    Format("%s%s", grid_position.Top, Call(layout.Dimension, cell_pos_y)),
		Right =  Format("%s%s", grid_position.Left, Call(layout.Dimension, cell_pos_x + layout.Width)),
		Bottom = Format("%s%s", grid_position.Top, Call(layout.Dimension, cell_pos_y + layout.Height))
	};
}

/**
 * Adds the contents of one proplist to the other.
 * Usually you can do this by using a prototype, but 
 * this is not possible when you receive a second proplist
 * from another function.
 * 
 * @par original This proplist will be expanded.
 * @par additional These are the additional contents.
 * @par no_overwrite By default the function overwrites
 *      existing properties in the original proplist,
 *      as it would happen if you derive from a prototype.
 *      If this parameter is 'true' the function will report
 *      an error instead of overwriting a parameter.
 *
 * @return proplist The original proplist. This is in fact the same
 *         proplist that was passed as an argument, the functions just
 *         returns it for convenience.
 */
public func AddProperties(proplist original, proplist additional, bool no_overwrite)
{
	if (original == nil)
	{
		FatalError("Cannot add properties to 'nil'");
	}
	if (additional == nil)
	{
		FatalError("Adding proplist 'nil' to another proplist makes no sense.");
	}
	
	for (var property in GetProperties(additional))
	{
		if (no_overwrite && (original[property] != nil)) // about to overwrite?
		{
			FatalError(Format("Cancelling overwrite of property '%s', original value %v, with new value %v", property, original[property], additional[property]));
		}
		else
		{
			original[property] = additional[property];
		}
	}
	
	return original;
}
