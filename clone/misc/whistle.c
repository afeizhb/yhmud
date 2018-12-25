/* whistle.c ���� */

#include <ansi.h>

#define RIDE_CMD              "/cmds/std/ride"
#define UNRIDE_CMD            "/cmds/std/unride"

inherit ITEM;

void init()
{
    if ( interactive( this_player() ) &&
         environment() == this_player() )
    {
        add_action( "do_whistle", "whistle" );
    }
}

void create()
{
    set_name( HIG "����" NOR, ({ "ma shao", "shao" }) );
    if ( clonep() )
        set_default_object( __FILE__ );
    else {
        set( "unit", "��" );
        set( "long", HIC "����һ���������ڣ���Ҫ��ʱ��(whistle)һ�¾����ˡ�\n" NOR );
        set( "family", 0 );
        set( "whistle", 10 );
    }

    setup();
}

int do_whistle()
{
    object  horse;
    string  err, file;
    object  me  = this_player();
    object  ob  = this_object();

    if ( me->is_busy() || me->query( "doing" ) )
        return(notify_fail( "��������æ�أ������п�����˵�ɡ�\n" ) );

    if ( me->is_in_prison() )
        return(notify_fail( "�����������أ����ʲô����\n" ) );

    if ( me->is_ghost() )
        return(notify_fail( "���㻹�������ٻ��ɡ�\n" ) );

    if ( ob->query("family") && me->query( "family/family_name" ) != "������" )
        return(notify_fail( "���ֲ�����Զ�ھֵ��ˣ�û����ƥ�ɺ�����\n" ) );

    if ( environment( me )->query( "no_ride" ) ||
         environment( me )->query( "maze" ) )
        return(notify_fail( "�㷢�������е�Ź֣����ڵ�����������ȥ��\n" ) );

    if ( !environment( me )->query( "outdoors" ) )
        return(notify_fail( "�㻹�ǵ������ٺ�����ƥ�ɣ�\n" ) );

    if ( environment( me )->query( "outdoors" ) == "wuguan" )
        return(notify_fail( "����������˹���������ô������Ҳû�ã�\n" ) );

    if (me->query_temp( "is_riding" ))
        return(notify_fail( "�㲻������������\n"));

    if (sizeof(filter_array(all_inventory(environment(me)),
                            (: $1->query("ridable") :))))
        return(notify_fail( "�������Ѿ������������ˣ������ٺ����ɣ�\n"));

    switch ( random( 3 ) )
    {
    case 0:
        file = "/clone/horse/huangbiaoma";
        break;
    case 1:
        file = "/clone/horse/zaohongma";
        break;
    default:
        file = "/clone/horse/ziliuma";
    }
    call_other( file, "???" );
    err = catch( horse = new( file ) );
    if ( err || !ob->query( "whistle" ))
    {
        message_vision( HIM "$N" HIM "����һ�����ڣ�"
                "Ȼ��ʲôҲû�з��� :)\n" NOR, me );
        return(1);
    }
    horse->move( environment( me ) );
    horse->reset_action();
    horse->set_leader( me );
    message_vision( HIC "$N" HIC "�ó����ڴ���һ����"
            "һƥ$n" HIC "��Զ�����˹�����\n" NOR, me, horse );
    ob->add( "whistle", -1 );
    RIDE_CMD->main(me, horse->query("id"));

    if ( me->is_fighting() )
        me->start_busy( 1 + random( 3 ) );

    return(1);
}