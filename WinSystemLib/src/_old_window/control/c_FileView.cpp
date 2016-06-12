#if 0

#include <BaseLibOld.h>
#include "c_FileView.h"
#include "f_window.h"
#include <StringLib.h>
#include "f_base.h"


FileView::FileView(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: ListView(x,y,w,h,_parent,_option | WINDOWF_TABSTOP,_id), file(FOLDER_PERSONAL), imagelist(16,16,2,2)
{
	sort_column=0;
	sort_az=true;
	//イメージリスト作成
	Library *shell32=new Library(L"shell32.dll");
	IDIL_FOLDER=imagelist.addIcon(shell32,4);
 	IDIL_FILE=imagelist.addIcon(shell32,1);
	delete shell32;
	//カラム作成
	setImageList(imagelist);
	insertColumn(0,L"名前",140,-1,true);
	insertColumn(1,L"サイズ",80,1,false);
	insertColumn(2,L"種類",60,-1,false);
	insertColumn(3,L"作成日時",120,-1,false);
	insertColumn(4,L"アクセス日時",120,-1,false);
	insertColumn(5,L"更新日時",120,-1,false);	
	messageNotify(true);
}

FileView::~FileView()
{
}

void FileView::chDir(const wstring& dir)
{
	file.chDir(dir.c_str());
	realizeDir();
}


void FileView::setDir(const wstring& dir)
{
	file.setDir(dir.c_str());
	realizeDir();
}

void FileView::realizeDir()
{
	clear(); //リストビューのクリア
	file.setTitle(L"*.*");
	filelist.clear();
	int i=0,index;
	if(file.findFirst())do{
		filelist.put(&file,-1);
		index=addItem(0,LPSTR_TEXTCALLBACK,true,I_IMAGECALLBACK,i++);
		setItem(index,1,LPSTR_TEXTCALLBACK,false,0);
		setItem(index,2,LPSTR_TEXTCALLBACK,false,0);
		setItem(index,3,LPSTR_TEXTCALLBACK,false,0);
		setItem(index,4,LPSTR_TEXTCALLBACK,false,0);
		setItem(index,5,LPSTR_TEXTCALLBACK,false,0);
		if(file.isHidden())ListView_SetItemState(hwndList,index,LVIS_CUT,0xFFFFFFFF);
	}while(file.findNext());
	file.findClose();
	sort(sort_column,sort_az);
	ListView_SetItemState(hwndList,0,LVIS_FOCUSED,0xFFFFFFFF);
}

string FileView::getDir()
{
	return file.getDir();
}


static int CALLBACK CompProc(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{
	FileView *fileview=(FileView*)lParamSort;
	return fileview->compareProc(lParam1,lParam2);
}

int FileView::compareProc(LPARAM lParam1,LPARAM lParam2)
{
	FileListElement *file1=filelist.get(lParam1);
	FileListElement *file2=filelist.get(lParam2);
	int ret1,ret2,ret=0,azpower=sort_az?1:-1;
	// -- -- ファイルとフォルダの分別 -- -- //
	ret1=file1->isFolder()?0:1;
	ret2=file2->isFolder()?0:1;
	ret=ret1-ret2; if(ret!=0)return ret*azpower;
	// -- -- ファイル同士・フォルダ同士のチェック -- -- //
	switch(sort_column){
	case 0:  // -- -- 名前（下で行うので省略） -- -- //
		break;
	case 1:  // -- -- サイズ -- -- //
		ret=file1->GetSize()-file2->GetSize(); break;
	case 2:  // -- -- 種類 -- -- //
		ret=stricmp(file1->getExt().c_str(),file2->getExt().c_str()); break;
	case 3:  // -- -- 作成日時 -- -- //
		ret=file1->getCreateTime()->compare(file2->getCreateTime()); break;
	case 4:  // -- -- アクセス日時 -- -- //
		ret=file1->getAccessTime()->compare(file2->getAccessTime()); break;
	case 5:  // -- -- 更新日時 -- -- //
		ret=file1->getWriteTime()->compare(file2->getWriteTime()); break;
	}
	// -- -- 順位が付かなかった場合は、名前で対比する -- -- //
	if(ret==0)
		ret=stricmp(file1->getTitle().c_str(),file2->getTitle().c_str());
	return ret*azpower;
}

void FileView::sort(int _sort_column,bool _sort_az)
{
	sort_column=_sort_column;
	sort_az=_sort_az;
	sortItemsByFunc(CompProc,(LPARAM)this);
}


LRESULT FileView::onNotify(UINT msg,WPARAM wParam,LPARAM lParam)
{
	NMHDR *nmh=(NMHDR*)lParam;
	FileName tmpfile;
	if(nmh->hwndFrom==hwndList){
		if(nmh->code==NM_RCLICK){
			// ** ** 右クリック ** ** //
			NMLISTVIEW *nmclick=(NMLISTVIEW*)lParam;
			PopupMenu menu;
			menu.insertItem(0,L"開く",100);
			menu.insertItem(1,L"削除",101);
			int ret=menu.popup(this);
			if(ret!=0){
				MessageBox(hwnd,tmp_itoa(ret),L"t",MB_OK);
			}
			return 0L;
		}else if(nmh->code==NM_DBLCLK || nmh->code==LVN_KEYDOWN && ((NMLVKEYDOWN*)lParam)->wVKey==VK_RETURN){
			// ** ** ダブルクリック or ENTER ** ** //
			int index=getSelectedParam();
			if(index!=-1){
				tmpfile=*filelist.get(index);
				SendMessage(getParent()->getHWND(),WM_NOTIFY_FILEVIEW,KN_FILEVIEW_RETURN,(LPARAM)&tmpfile);
//				tmpfile=filelist.get(index);
//				if(tmpfile->isFolder()){
//					chDir(tmpfile->getTitle());
//					SendMessage(parent->getHWND(),WM_NOTIFY_FILEVIEW,KN_FILEVIEW_CHDIR,0);
//				}
			}
			return 0L;
		}else if(nmh->code==LVN_KEYDOWN){
			NMLVKEYDOWN *nmkey=(NMLVKEYDOWN*)lParam;
			if(nmkey->wVKey==VK_BACK){
				// ** ** BackSpace ** ** //
				tmpfile.setPath(L"..");
				tmpfile.toFolder();
				SendMessage(getParent()->getHWND(),WM_NOTIFY_FILEVIEW,KN_FILEVIEW_RETURN,(LPARAM)&tmpfile);
//				chDir(L"..");
//				SendMessage(parent->getHWND(),WM_NOTIFY_FILEVIEW,KN_FILEVIEW_CHDIR,0);
			}
			return 0L;
		}else if(nmh->code==NM_RCLICK){
			// ** ** 右クリック ** ** //
			ChainList<int> *list=getSelectedParams();
			int n;
			if(list->eachFirst(&n))do{
				MessageBox(hwnd,filelist.get(n)->getTitle().c_str(),L"t",MB_OK);
			}while(list->eachNext(&n));
			list->eachClose();
			SetFocus(hwnd);
			return 0L;
		}else if(nmh->code==LVN_COLUMNCLICK){
			// ** ** ソート ** ** //
			NMLISTVIEW *nmlv=(NMLISTVIEW*)lParam;
			if(nmlv->iSubItem==sort_column){
				sort(sort_column,!sort_az);
			}else{
				sort(nmlv->iSubItem,true);
			}
			return 0L;
		}else if(nmh->code==LVN_GETDISPINFO){
			// ** ** アイテム情報 ** ** //
			NMLVDISPINFO *nmdi=(NMLVDISPINFO*)nmh;
			FileListElement *file=filelist.get(nmdi->item.lParam);
			LVITEM *item=&nmdi->item;
			//
			switch(nmdi->item.iSubItem){
			case 0: // -- -- ファイル名 -- -- //
				if(nmdi->item.mask & LVIF_TEXT){
					strcpy(item->pszText,file->getTitle().c_str());
				}
				if(nmdi->item.mask & LVIF_IMAGE){
					item->iImage=file->isFolder()?IDIL_FOLDER:IDIL_FILE;
				}
				break;
			case 1: // -- -- サイズ -- -- //
				if(nmdi->item.mask & LVIF_TEXT){
					if(!file->isFolder())
						strcpy(item->pszText,tmp_itoa_comma(file->GetSize(),3));
					else
						strcpy(item->pszText,L"");
				}
				break;
			case 2: // -- -- 種類 -- -- //
				if(nmdi->item.mask & LVIF_TEXT){
					if(!file->isFolder())
						uppercpy(item->pszText,file->getExt().c_str());
					else
						strcpy(item->pszText,L"フォルダ");
				}
				break;
			case 3: // -- -- 作成日時 -- -- //
				if(nmdi->item.mask & LVIF_TEXT){
					strcpy(item->pszText,file->getCreateTime()->toString());
				}
				break;
			case 4: // -- -- アクセス日時 -- -- //
				if(nmdi->item.mask & LVIF_TEXT){
					strcpy(item->pszText,file->getAccessTime()->toString());
				}
				break;
			case 5: // -- -- 更新日時 -- -- //
				if(nmdi->item.mask & LVIF_TEXT){
					strcpy(item->pszText,file->getWriteTime()->toString());
				}
				break;
			}
			return 0L;
		}
	}
	return Window::onNotify(msg,wParam,lParam);
}


#endif

